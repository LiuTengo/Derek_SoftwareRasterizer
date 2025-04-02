#include "../RendererSettings.h"

int RendererSettings::WINDOW_WIDTH = 500;
int RendererSettings::WINDOW_HEIGHT = 500;

Matrix4X4f RendererSettings::vp = Matrix4X4f::Identity();
Matrix4X4f RendererSettings::m = Matrix4X4f::Identity();
Matrix4X4f RendererSettings::mvp = Matrix4X4f::Identity();

float RendererSettings::LightIntensity = 0.0f;
Vector3f RendererSettings::LightPosition = Vector3f{0,0,0};
Vector3f RendererSettings::LightColor = Vector3f{ 1,1,1 };
std::vector<double> RendererSettings::ShadowMap = std::vector<double>(1,1.0f);
Matrix4X4f RendererSettings::lightVP = Matrix4X4f::Identity();

int RendererSettings::ShadowMapWidth = 500;
int RendererSettings::ShadowMapHeight = 500;
float RendererSettings::lightNearPlane = 0.1;
float RendererSettings::lightFarPlane = 1;

Vector3f RendererSettings::CameraPosition = Vector3f{0,0,0};
Vector3f RendererSettings::CameraDirection = Vector3f{ 1,0,0 };

Vector4f RendererSettings::GetLightCoordinate(const Vector3f& worldPosition)
{
	Vector4f worldPos = worldPosition.toVector4();
	Vector4f clipPos = RendererSettings::lightVP*worldPos;

	clipPos = clipPos / clipPos.w(); // NDC¿Õ¼ä

	return clipPos;
}

double RendererSettings::LightDepth(double z)
{
	double inv_z = 1. / z;
	return inv_z * ((RendererSettings::lightNearPlane - z) * RendererSettings::lightFarPlane) / (RendererSettings::lightNearPlane - RendererSettings::lightFarPlane);
}
