#pragma once
#include "../Math/Vector.hpp"

#include "Material.h"

class Shader
{
public:
	Shader();
	virtual ~Shader();
	FragmentInput virtual VertexShader(const VertexInput& input);
	Vector3f virtual FragmentShader(const FragmentInput& input);
};

