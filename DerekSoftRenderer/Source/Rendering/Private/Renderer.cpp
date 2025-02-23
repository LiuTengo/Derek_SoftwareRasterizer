#include "../Renderer.h"

Renderer::Renderer()
{
	int totalPixel = RendererSettings::WINDOW_WIDTH* RendererSettings::WINDOW_HEIGHT;
	frameBuffer = std::vector<Vector3f>(totalPixel, Vector3f{0.0f,0.0f,0.0f});
	depthBuffer = std::vector<float>(totalPixel, std::numeric_limits<float>::infinity());
}

void Renderer::ClearDepth()
{
	std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::infinity());
}

void Renderer::ClearColor()
{
	std::fill(frameBuffer.begin(),frameBuffer.end(),Vector3f{0,0,0});
}

bool Renderer::IsInTriangle(const Triangle& t, float x, float y)
{
	Vector3f v[3];
	for (int i = 0; i < 3;i++) {
		v[i] = Vector3f{t.points[i][0],t.points[i][1],1.0f};
	}
	
	Vector3f v1 = v[1].cross(v[0]);
	Vector3f v2 = v[2].cross(v[1]);
	Vector3f v3 = v[0].cross(v[2]);
	Vector3f p = Vector3f{x,y,1.0f};
	if ((p.dot(v1) * v1.dot(v[2]) > 0) && (p.dot(v2) * v2.dot(v[0]) > 0) && (p.dot(v3) * v3.dot(v[1]) > 0))
		return true;
	return false;
}

void Renderer::Draw(const HDC& hdc, Scene* scene)
{
	if (scene == nullptr) {
		return;
	}
	//更新VP矩阵
	//mvp = ...
	
	//清除Buffer
	ClearDepth();
	ClearColor();

	//foreach light in scene.lightList{
	//	foreach obj in scene.objectList{
	//		obj.Draw(light);
	//	}
	// }
	for(auto obj : scene->objectArray) {
		//应用顶点着色器
		obj->UseVertexShader();
		//裁剪
		
		//映射到屏幕坐标
		
		//光栅化
		Rasterize(obj);
	}
	//混合阶段？

	//将每帧buffer写入HDC
	CopyBufferToHDC(hdc); 
}

void Renderer::Rasterize(Object* obj)
{
	for (const auto& triangle : obj->triangleList) {
		auto v = triangle.toVector4();

		int xMax = max(triangle.points[0].x(), max(triangle.points[1].x(), triangle.points[2].x()));
		int xMin = min(triangle.points[0].x(), min(triangle.points[1].x(), triangle.points[2].x()));
		int yMax = max(triangle.points[0].y(), max(triangle.points[1].y(), triangle.points[2].y()));
		int yMin = min(triangle.points[0].y(), min(triangle.points[1].y(), triangle.points[2].y()));

		for (int x = xMin; x < xMax; x++) {
			for (int y = yMin; y < yMax; y++) {
				if (IsInTriangle(triangle, (float)x+0.5f, (float)y+0.5f)) {
					//计算重心坐标
					float alpha, beta, gamma;
					std::tie(alpha,beta,gamma) = ComputeBarycentricCoordinate((float)x + 0.5f, (float)y + 0.5f, triangle.points);
					float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
					float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
					z_interpolated *= w_reciprocal;

					//插值

					int index = GetBufferIndex(x,y);
					if (DepthTest(index, z_interpolated)) {
						//应用像素着色器
						frameBuffer[index] = obj->UseFragmentShader();
					}
				}
			}
		}
	}
}

void Renderer::CopyBufferToHDC(const HDC& hdc)
{
	for (int x = 0; x < RendererSettings::WINDOW_WIDTH; x++) {
		for (int y = 0; y < RendererSettings::WINDOW_HEIGHT; y++) {
			int index = GetBufferIndex(x, y);
			auto col = 255.0f *frameBuffer[index];
			SetPixel(hdc, x, y, RGB(col[0], col[1], col[2]));
		}
	}
}

int Renderer::GetBufferIndex(int x,int y)
{
	return y * RendererSettings::WINDOW_WIDTH + x;
}

bool Renderer::DepthTest(const int& index, const float& depth)
{
	if (depth < depthBuffer[index]) {
		depthBuffer[index] = depth;
		return true;
	}
	return false;
}

std::tuple<float, float, float> Renderer::ComputeBarycentricCoordinate(float x, float y, const Vector4f* v)
{
	float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
	float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
	float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) / (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() - v[1].x() * v[0].y());
	return { c1,c2,c3 };
}