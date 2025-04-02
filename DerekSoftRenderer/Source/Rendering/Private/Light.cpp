#include "../Light.h"

Light::Light()
{
	Position = Vector3f{ 7,5,5 }; 
	Direction = Vector3f::normalize(Vector3f{ 0,0,0 } - Position),
	Color = Vector3f{1,1,1};
	Intensity = 0.8f;
	ShadowMapHeight = 0;
	ShadowMapWidth = 0;
}

void Light::SetShadwMap(const std::vector<MeshObject*>& objList, int width, int height)
{
	//Set VirtualCamera
	ShadowMapWidth = width;
	ShadowMapHeight = height;
	RendererSettings::ShadowMapWidth = width;
	RendererSettings::ShadowMapHeight = height;

	depthBuffer = std::vector<float>(width*height,std::numeric_limits<float>::infinity());
	ShadowMap = std::vector<BYTE>(width * height*3,BYTE(0));
	RendererSettings::ShadowMap = std::vector<double>(width * height, std::numeric_limits<double>::infinity());


	std::unique_ptr<Camera> virtualCamera = std::make_unique<Camera>
		(Camera(Position, Vector3f{ 0,0,0 },30.0f,0.1f, 60.0f,(float)width / (float)height));

	RendererSettings::lightFarPlane = virtualCamera->farPlane;
	RendererSettings::lightNearPlane = virtualCamera->nearPlane;
	RendererSettings::lightVP = virtualCamera->GetProjectionMatrix()* virtualCamera->GetViewMatrix();

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
				v.clipPosition.y() = (v.clipPosition.y() * 0.5 + 0.5) * ShadowMapHeight;
				v.clipPosition.z() = virtualCamera->GetNonLinearDepth(v.clipPosition.z());
			}

			//写入ShadowMap
			for (int i = 0; i < 3; i++) {
				newTri.SetVertexData(i, fragInput[i]);
			}
			auto v = newTri.toVector4();

			float xMaxf = 0;
			float xMinf = RendererSettings::ShadowMapWidth;
			float yMaxf = 0;
			float yMinf = RendererSettings::ShadowMapHeight;

			for (int i = 0; i < 3; i++) {
				xMaxf = max(newTri.vertices[i].clipPoints.x(), xMaxf);
				xMinf = min(newTri.vertices[i].clipPoints.x(), xMinf);
				yMaxf = max(newTri.vertices[i].clipPoints.y(), yMaxf);
				yMinf = min(newTri.vertices[i].clipPoints.y(), yMinf);
			}

			//Clamp
			int xMax = ceil(xMaxf);
			xMax = xMax > RendererSettings::ShadowMapWidth ? RendererSettings::ShadowMapWidth : xMax;
			int xMin = floor(xMinf);
			xMin = xMin < 0 ? 0 : xMin;
			int yMax = ceil(yMaxf);
			yMax = yMax > RendererSettings::ShadowMapHeight ? RendererSettings::ShadowMapHeight : yMax;
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

						int index = (ShadowMapHeight - y - 1) * ShadowMapWidth + x;
						if (depthBuffer[index] > z_interpolated) {
							depthBuffer[index] = z_interpolated;

							ShadowMap[index*3 + 0] = z_interpolated * 255;
							ShadowMap[index*3 + 1] = z_interpolated * 255;
							ShadowMap[index*3 + 2] = z_interpolated * 255;
							RendererSettings::ShadowMap[index] = z_interpolated;
						}
					}
				}
			}
		}
	}
}
