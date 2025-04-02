#pragma once
#include <vector>
#include "../Math/Matrix.hpp"

static class RendererSettings
{
public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static const int TimeStep = 30;

public:
	static Matrix4X4f vp;
	static Matrix4X4f m;
	static Matrix4X4f mvp;

public:
	static float LightIntensity;
	static Vector3f LightColor;
	static Vector3f LightPosition;
	static std::vector<double> ShadowMap;
	static float lightNearPlane;
	static float lightFarPlane;
	static Matrix4X4f lightVP;
	static int ShadowMapWidth;
	static int ShadowMapHeight;

public:
	static Vector3f CameraPosition;
	static Vector3f CameraDirection;

public:
	static Vector4f GetLightCoordinate(const Vector3f& worldPosition);
	static double LightDepth(double z);
};