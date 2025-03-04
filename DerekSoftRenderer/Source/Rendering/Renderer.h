#pragma once

#include <windows.h>
#include "../Math/Matrix.hpp"
#include "../Gameplay/MeshObject.h"
#include "../Gameplay/Scene.h"
#include "Triangle.h"
#include "RendererSettings.h"

class Scene;

class Renderer
{
public:
	Renderer();
	~Renderer();
private:
	std::vector<BYTE> frameBuffer;
	std::vector<float> depthBuffer;
	Matrix4X4f vp;
	Matrix4X4f mvp;
	BITMAPINFO bmi;
public:
	void ClearDepth();
	void ClearColor();
	bool IsInTriangle(const Triangle& t,float x,float y);
	//ªÊ÷∆√¸¡Ó
	void Draw(const HDC& hdc,Scene* scene = nullptr);

	//π‚’§ªØ
	void Rasterize(MeshObject* obj,Triangle triangle);

	void CopyBufferToHDC(const HDC& hdc);
private:
	int GetBufferIndex(int x, int y);
	Vector2f Interpolate(float alpha, float beta, float gamma, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, float weight = 1);
	Vector3f Interpolate(float alpha,float beta,float gamma,const Vector3f& v1, const Vector3f& v2, const Vector3f& v3,float weight = 1);
	Vector4f Interpolate(float alpha, float beta, float gamma, const Vector4f& v1, const Vector4f& v2, const Vector4f& v3, float weight = 1);
	bool DepthTest(const int& index, const float& depth);
	Vector3f Clamp(const Vector3f& vec);
	void SetFrameBuffer(int index,const Vector3f& res);
	std::tuple<float, float, float> ComputeBarycentricCoordinate(float x, float y, const Vertex* v);
};