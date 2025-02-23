#pragma once
#include <iostream>

#include "Shader.h"
#include "Triangle.h"
#include "../Math/Vector.hpp"

struct VertexInput {
	Vector4f modelPosition;
	Vector3f normal;
	Vector3f color;
	Vector2f uv;
};

struct FragmentInput {
	Vector4f clipPosition;
	Vector3f worldPosition;
	Vector3f worldNormal;
	Vector2f uv;
};

class Material
{
public:
	Material(const Shader*& shader);
	~Material();

public:
	std::shared_ptr<Shader> shader;

	VertexInput vertInput;
	FragmentInput fragInput;

public:
	void SetVertexInputData(const Triangle& triangle);
	void ApplyVertexShader();
	Vector3f ApplyFragmentShader();
};