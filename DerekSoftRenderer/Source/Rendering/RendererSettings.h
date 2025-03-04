#pragma once

#include "../Math/Matrix.hpp"

static class RendererSettings
{
public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static const int TimeStep = 30;

public:
	static Matrix4X4f v;
	static Matrix4X4f vp;
	static Matrix4X4f mv;
	static Matrix4X4f mvp;

public:
	static float LightIntensity;
	static Vector3f LightColor;
	static Vector4f LightPosition;

public:
	static Vector3f CameraDirection;
};