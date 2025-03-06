#include "../Light.h"

Light::Light()
{
	Position = Vector4f{ -8,0,8,1 };
	Direction = Vector3f::normalize(Vector3f{0,0,0} - Position.toVector3());
	Color = Vector3f{1,1,1};
	Intensity = 1.f;
}

void Light::SetShadwMap(const std::vector<MeshObject*>& objList, int width, int height)
{
	//Set VirtualCamera
	ShadowMapWidth = width;
	ShadowMapHeight = height;
	depthBuffer = std::vector<float>(width*height,std::numeric_limits<float>::infinity());
	ShadowMap = std::vector<BYTE>(width * height*3,BYTE());

	std::unique_ptr<Camera> virtualCamera = std::make_unique<Camera>(Camera());
	virtualCamera->position = Position.toVector3();
	virtualCamera->cameraDirection = Direction;
	virtualCamera->aspectRatio = (float)width / (float)height;
	Matrix3X3f rotation = Matrix3X3f
		{{0, 1, 0  },
		 {0, 0, -1 },
		 {1, 0, 0} };
	virtualCamera->cameraUp = rotation * Direction;
	virtualCamera->cameraRight = Vector3f::normalize(virtualCamera->cameraDirection.cross(virtualCamera->cameraUp));


	//Render ShadowMap
	for (auto obj : objList) {

		RendererSettings::mvp = virtualCamera->GetVPMatrix() * obj->GetModelMatrix();

		for (const auto& triangle : obj->triangleList) {

			Triangle newTri = *triangle;

			std::array<FragmentInput, 3> fragInput;
			for (int i = 0; i < 3; i++) {
				VertexInput vertPayload = VertexInput(triangle->vertices[i]);
				//应用顶点着色器
				fragInput[i] = obj->UseVertexShaderProgram(vertPayload);
			}

			for (auto& v : fragInput) {
				//Homogenuos division
				v.clipPosition = v.clipPosition / (v.clipPosition.w());
				//Viewport Transformation
				v.clipPosition.x() = (v.clipPosition.x() * 0.5 + 0.5) * ShadowMapWidth;
				v.clipPosition.y() = (v.clipPosition.y() * 0.5 + 0.5) * ShadowMapHeight;//0.5 * RendererSettings::WINDOW_HEIGHT * (v.clipPosition.y() + 1);
				v.clipPosition.z() = (v.clipPosition.z() - 0.1f) / (virtualCamera->GetFarSubstractNear());//(v.clipPosition.z()*f1+f2);
			}

			//写入ShadowMap
			for (int i = 0; i < 3; i++) {
				newTri.SetVertexData(i, fragInput[i]);
			}
			auto v = newTri.toVector4();

			float xMaxf = 0;
			float xMinf = RendererSettings::WINDOW_WIDTH;
			float yMaxf = 0;
			float yMinf = RendererSettings::WINDOW_HEIGHT;

			for (int i = 0; i < 3; i++) {
				xMaxf = max(newTri.vertices[i].clipPoints.x(), xMaxf);
				xMinf = min(newTri.vertices[i].clipPoints.x(), xMinf);
				yMaxf = max(newTri.vertices[i].clipPoints.y(), yMaxf);
				yMinf = min(newTri.vertices[i].clipPoints.y(), yMinf);
			}

			//Clamp
			int xMax = ceil(xMaxf);
			xMax = xMax > RendererSettings::WINDOW_WIDTH ? RendererSettings::WINDOW_WIDTH : xMax;
			int xMin = floor(xMinf);
			xMin = xMin < 0 ? 0 : xMin;
			int yMax = ceil(yMaxf);
			yMax = yMax > RendererSettings::WINDOW_HEIGHT ? RendererSettings::WINDOW_HEIGHT : yMax;
			int yMin = floor(yMinf);
			yMin = yMin < 0 ? 0 : yMin;

			for (int x = xMin; x < xMax; x++) {
				for (int y = yMin; y < yMax; y++) {
					if (Renderer::IsInTriangle(newTri, (float)x + 0.5f, (float)y + 0.5f)) {
						//计算重心坐标
						float alpha, beta, gamma;
						std::tie(alpha, beta, gamma) = Renderer::ComputeBarycentricCoordinate((float)x + 0.5f, (float)y + 0.5f, newTri.vertices);
						float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
						float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
						z_interpolated *= w_reciprocal;

						//int index = GetBufferIndex(x, y);
						int index = y*ShadowMapWidth + x;
						if (depthBuffer[index] > z_interpolated) {
							depthBuffer[index] = z_interpolated;
							
							float num = -z_interpolated;
							ShadowMap[index*3 + 0] = num * 255;
							ShadowMap[index*3 + 1] = num * 255;
							ShadowMap[index*3 + 2] = num * 255;
						}
					}
				}
			}
		}
	}
}
