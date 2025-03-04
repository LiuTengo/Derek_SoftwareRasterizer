#pragma once
#include "../Math/Vector.hpp"
#include "../Math/Matrix.hpp"
#include "Vertex.hpp"

class Shader
{
public:
	Shader();
	virtual ~Shader();
	FragmentInput virtual VertexShader(const VertexInput& input);
	Vector3f virtual FragmentShader(const FragmentInput& input);
};

