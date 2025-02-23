#include "../Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

FragmentInput Shader::VertexShader(const VertexInput& input)
{
	return FragmentInput();
}

Vector3f Shader::FragmentShader(const FragmentInput& input)
{
	return Vector3f();
}