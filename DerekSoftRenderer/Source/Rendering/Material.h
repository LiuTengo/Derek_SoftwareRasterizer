#pragma once
#include <iostream>

#include "Shader.h"
#include "Triangle.h"
#include "../Math/Vector.hpp"

class Material
{
public:
	Material()=default;
	Material(Shader* shader);
	~Material();

public:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;

public:
	FragmentInput ApplyVertexShader(const VertexInput& input);
	Vector3f ApplyFragmentShader(FragmentInput& input);

	void SetTexture(Texture* texture);
};