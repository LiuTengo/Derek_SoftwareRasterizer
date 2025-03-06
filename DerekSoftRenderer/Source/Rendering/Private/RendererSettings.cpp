#include "../RendererSettings.h"

int RendererSettings::WINDOW_WIDTH = 500;
int RendererSettings::WINDOW_HEIGHT = 500;

Matrix4X4f RendererSettings::v = Matrix4X4f::Identity();
Matrix4X4f RendererSettings::vp = Matrix4X4f::Identity();
Matrix4X4f RendererSettings::m = Matrix4X4f::Identity();
Matrix4X4f RendererSettings::mvp = Matrix4X4f::Identity();

float RendererSettings::LightIntensity = 0.0f;
Vector4f RendererSettings::LightPosition = Vector4f{0,0,0,1};
Vector3f RendererSettings::LightColor = Vector3f{ 1,1,1 };

Vector3f RendererSettings::CameraDirection = Vector3f{0,0,0};