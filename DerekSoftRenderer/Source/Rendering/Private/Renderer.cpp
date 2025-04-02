#include "../Renderer.h"

Renderer::Renderer()
{
	int totalPixel = RendererSettings::WINDOW_WIDTH* RendererSettings::WINDOW_HEIGHT;
	frameBuffer = std::vector<BYTE>(totalPixel*3, BYTE());
	depthBuffer = std::vector<float>(totalPixel, std::numeric_limits<float>::infinity());

	bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = RendererSettings::WINDOW_WIDTH;
	bmi.bmiHeader.biHeight = -RendererSettings::WINDOW_HEIGHT; // 负值表示顶部为起点 (防止上下翻转)
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;  // 24-bit BGR
	bmi.bmiHeader.biCompression = BI_RGB;  // 无压缩
}

Renderer::~Renderer()
{
	OutputDebugString((L"Close Window\n"));
}

void Renderer::ClearDepth()
{
	std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::infinity());
}

void Renderer::ClearColor()
{
	std::fill(frameBuffer.begin(),frameBuffer.end(), BYTE());
}

bool Renderer::IsInTriangle(const Triangle& t, float x, float y)
{
	Vector3f v[3];
	for (int i = 0; i < 3;i++) {
		v[i] = Vector3f{t.vertices[i].clipPoints.x(),t.vertices[i].clipPoints.y(),1};
	}
	Vector3f p = Vector3f{x,y,1.0f};
	Vector3f v12 = v[1] - v[0];
	Vector3f v13 = v[2] - v[0];
	Vector3f v23 = v[2] - v[1];
	Vector3f p1 = p - v[0];
	Vector3f p2 = p - v[1];
	Vector3f p3 = p - v[2];
	
	Vector3f v1 = v12.cross(p1);
	Vector3f v2 = v23.cross(p2);
	Vector3f v3 = p3.cross(v13);
	
	if ((v1.z() > 0 && v2.z() > 0 && v3.z() > 0) || (v1.z() < 0 && v2.z() < 0 && v3.z() < 0))
	{
		return true;
	}
	else return false;
}

void Renderer::Draw(const HDC& hdc, const std::vector<MeshObject*>& objArray, const std::unique_ptr<Camera>& camera)
{
	//更新VP矩阵
	RendererSettings::vp = camera->GetVPMatrix();
	RendererSettings::CameraDirection = camera->cameraDirection;
	//清除Buffer
	ClearDepth();
	ClearColor();

	for(auto obj : objArray) {

		RendererSettings::m = obj->GetModelMatrix();
		RendererSettings::mvp = RendererSettings::vp * RendererSettings::m;

		for (const auto& triangle : obj->triangleList) {

			Triangle newTri = *triangle;

			std::array<FragmentInput, 3> fragInput;
			for (int i = 0; i < 3;i++) {
				VertexInput vertPayload = VertexInput(triangle->vertices[i]);
				//应用顶点着色器
				fragInput[i] = obj->UseVertexShaderProgram(vertPayload);
			}
			
			for (auto& v : fragInput) {
				//Homogenuos division
				v.clipPosition = v.clipPosition / (v.clipPosition.w());
				//Viewport Transformation
				v.clipPosition.x() = (v.clipPosition.x() * 0.5 + 0.5) * RendererSettings::WINDOW_WIDTH;
				v.clipPosition.y() = (v.clipPosition.y() * 0.5 + 0.5) * RendererSettings::WINDOW_HEIGHT;//0.5 * RendererSettings::WINDOW_HEIGHT * (v.clipPosition.y() + 1);
				v.clipPosition.z() = camera->GetNonLinearDepth(v.clipPosition.z());
			}

			for (int i = 0; i < 3;i++) {
				newTri.SetVertexData(i, fragInput[i]);
			}

			//光栅化
			Rasterize(obj, newTri);
		}
	}
}

void Renderer::Rasterize(MeshObject* obj,Triangle triangle)
{
	//cull back face
	Vector3f triNormal = triangle.GetTriangleNormal();
	if (triNormal.dot(RendererSettings::CameraDirection) < 0) {
		return;
	}

	auto v = triangle.toVector4();

	float xMaxf = 0;
	float xMinf = RendererSettings::WINDOW_WIDTH;
	float yMaxf = 0;
	float yMinf = RendererSettings::WINDOW_HEIGHT;

	for (int i = 0; i < 3;i++) {
		xMaxf = max(triangle.vertices[i].clipPoints.x(), xMaxf);
		xMinf = min(triangle.vertices[i].clipPoints.x(), xMinf);
		yMaxf = max(triangle.vertices[i].clipPoints.y(), yMaxf);
		yMinf = min(triangle.vertices[i].clipPoints.y(), yMinf);
	}

	//Clamp
	int xMax = ceil(xMaxf > RendererSettings::WINDOW_WIDTH ? RendererSettings::WINDOW_WIDTH : xMaxf);
	int xMin = floor(xMinf < 0 ? 0 : xMinf);
	int yMax = ceil(yMaxf > RendererSettings::WINDOW_HEIGHT ? RendererSettings::WINDOW_HEIGHT : yMaxf);
	int yMin = floor(yMinf < 0 ? 0 : yMinf);

	for (int x = xMin; x < xMax; x++) {
		for (int y = yMin; y < yMax; y++) {
			if (IsInTriangle(triangle, (float)x+0.5f, (float)y+0.5f)) {
				//计算重心坐标
				float alpha, beta, gamma;
				std::tie(alpha,beta,gamma) = ComputeBarycentricCoordinate((float)x + 0.5f, (float)y + 0.5f, triangle.vertices);
				float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
				float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
				z_interpolated *= w_reciprocal;

				int index = GetBufferIndex(x,y);
				if (DepthTest(index, z_interpolated)) {
					FragmentInput fragInput;
					//插值
					fragInput.clipPosition = Interpolate(alpha, beta, gamma, triangle.vertices[0].clipPoints, triangle.vertices[1].clipPoints, triangle.vertices[2].clipPoints);
					fragInput.worldPosition = Interpolate(alpha,beta,gamma,triangle.vertices[0].worldPosition, triangle.vertices[1].worldPosition, triangle.vertices[2].worldPosition);
					fragInput.worldNormal = Interpolate(alpha, beta, gamma, triangle.vertices[0].normal, triangle.vertices[1].normal, triangle.vertices[2].normal);
					fragInput.uv = Interpolate(alpha, beta, gamma, triangle.vertices[0].uv, triangle.vertices[1].uv, triangle.vertices[2].uv);
					//应用像素着色器
					Vector3f renderRes = Clamp(obj->UseFragmentShaderProgram(fragInput));
					SetFrameBuffer(index,renderRes);
				}
			}
		}
	}
}

void Renderer::CopyFrameBufferToHDC(const HDC& hdc)
{
	StretchDIBits(hdc, 0, 0, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT,
		0, 0, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT,
		frameBuffer.data(), &bmi, DIB_RGB_COLORS, SRCCOPY);
}

int Renderer::GetBufferIndex(int x,int y)
{
	//return y * RendererSettings::WINDOW_WIDTH + x;
	return (RendererSettings::WINDOW_HEIGHT - y-1)* RendererSettings::WINDOW_WIDTH + x;
}

Vector2f Renderer::Interpolate(float alpha, float beta, float gamma, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, float weight)
{
	return (alpha * v1 + beta * v2 + gamma * v3) / weight;
}

Vector3f Renderer::Interpolate(float alpha, float beta, float gamma, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, float weight)
{
	return (alpha*v1+beta*v2+gamma*v3)/weight;
}

Vector4f Renderer::Interpolate(float alpha, float beta, float gamma, const Vector4f& v1, const Vector4f& v2, const Vector4f& v3, float weight)
{
	return (alpha * v1 + beta * v2 + gamma * v3) / weight;
}

bool Renderer::DepthTest(const int& index, const float& depth)
{
	if (depth < depthBuffer[index]) {
		depthBuffer[index] = depth;
		return true;
	}
	return false;
}

Vector3f Renderer::Clamp(const Vector3f& vec)
{
	return Vector3f{max(min(vec.x(),1),0),max(min(vec.y(),1),0) ,max(min(vec.z(),1),0) };
}

void Renderer::SetFrameBuffer(int index, const Vector3f& res)
{
	//BGR模式 
	frameBuffer[index * 3 + 0] = res.z() * 255;
	frameBuffer[index * 3 + 1] = res.y() * 255;
	frameBuffer[index * 3 + 2] = res.x() * 255;
}

std::tuple<float, float, float> Renderer::ComputeBarycentricCoordinate(float x, float y, const Vertex* v)
{
	float c1 = (x * (v[1].clipPoints.y() - v[2].clipPoints.y()) + (v[2].clipPoints.x() - v[1].clipPoints.x()) * y + v[1].clipPoints.x() * v[2].clipPoints.y() - v[2].clipPoints.x() * v[1].clipPoints.y()) / (v[0].clipPoints.x() * (v[1].clipPoints.y() - v[2].clipPoints.y()) + (v[2].clipPoints.x() - v[1].clipPoints.x()) * v[0].clipPoints.y() + v[1].clipPoints.x() * v[2].clipPoints.y() - v[2].clipPoints.x() * v[1].clipPoints.y());
	float c2 = (x * (v[2].clipPoints.y() - v[0].clipPoints.y()) + (v[0].clipPoints.x() - v[2].clipPoints.x()) * y + v[2].clipPoints.x() * v[0].clipPoints.y() - v[0].clipPoints.x() * v[2].clipPoints.y()) / (v[1].clipPoints.x() * (v[2].clipPoints.y() - v[0].clipPoints.y()) + (v[0].clipPoints.x() - v[2].clipPoints.x()) * v[1].clipPoints.y() + v[2].clipPoints.x() * v[0].clipPoints.y() - v[0].clipPoints.x() * v[2].clipPoints.y());
	float c3 = (x * (v[0].clipPoints.y() - v[1].clipPoints.y()) + (v[1].clipPoints.x() - v[0].clipPoints.x()) * y + v[0].clipPoints.x() * v[1].clipPoints.y() - v[1].clipPoints.x() * v[0].clipPoints.y()) / (v[2].clipPoints.x() * (v[0].clipPoints.y() - v[1].clipPoints.y()) + (v[1].clipPoints.x() - v[0].clipPoints.x()) * v[2].clipPoints.y() + v[0].clipPoints.x() * v[1].clipPoints.y() - v[1].clipPoints.x() * v[0].clipPoints.y());
	return { c1,c2,c3 };
}