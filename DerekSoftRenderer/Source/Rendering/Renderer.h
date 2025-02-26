#pragma once

#include <windows.h>
#include "Triangle.h"
#include "RendererSettings.h"
#include "../Gameplay/Scene.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	std::vector<Vector3f> frameBuffer;
	std::vector<float> depthBuffer;
	Matrix4X4f vp;
	Matrix4X4f mvp;
public:
	void ClearDepth();
	void ClearColor();
	bool IsInTriangle(Triangle* t,float x,float y);
	void BeforeRendering(Scene* scene = nullptr);
	//ªÊ÷∆√¸¡Ó
	void Draw(const HDC& hdc,Scene* scene = nullptr);

	//π‚’§ªØ
	void Rasterize(MeshObject* obj);

	void CopyBufferToHDC(const HDC& hdc);
private:
	int GetBufferIndex(int x, int y);

	bool DepthTest(const int& index, const float& depth);

	std::tuple<float, float, float> ComputeBarycentricCoordinate(float x, float y, const Vertex* v);
};