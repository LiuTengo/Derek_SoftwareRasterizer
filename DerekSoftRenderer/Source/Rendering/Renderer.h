#pragma once

#include <windows.h>
#include "Triangle.h"
#include "../Math/Vector.hpp"
#include "RendererSettings.h"
#include "../Gameplay/Scene.h"

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	std::vector<Vector3f> frameBuffer;
	std::vector<float> depthBuffer;

public:
	void ClearDepth();
	void ClearColor();
	bool IsInTriangle(const Triangle& t,float x,float y);
	//ªÊ÷∆√¸¡Ó
	void Draw(const HDC& hdc,Scene* scene = nullptr);

	//π‚’§ªØ
	void Rasterize(Object* obj);

	void CopyBufferToHDC(const HDC& hdc);
private:
	int GetBufferIndex(int x, int y);

	bool DepthTest(const int& index, const float& depth);

	std::tuple<float, float, float> ComputeBarycentricCoordinate(float x, float y, const Vector4f* v);
};