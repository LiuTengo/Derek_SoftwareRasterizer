#pragma once

#include <vector>
#include <cmath>
#include "../Math/Vector.hpp"
#include "../Gameplay/Camera.h"
#include "../Gameplay/MeshObject.h"
#include "../Rendering/Renderer.h"

class Light
{
public:
	Light();

public:
	Vector4f Position;
	Vector3f Direction;
	Vector3f Color;
	float Intensity;

	int ShadowMapWidth;
	int ShadowMapHeight;
	std::vector<float> depthBuffer;
	std::vector<BYTE> ShadowMap;
public:
	void SetShadwMap(const std::vector<MeshObject*>& objList,int width,int height);
};

