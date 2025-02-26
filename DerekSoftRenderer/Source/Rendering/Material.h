#pragma once
#include <iostream>

#include "Shader.h"
#include "Triangle.h"
#include "../Math/Vector.hpp"

class Material
{
public:
	Material(Shader shader);
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