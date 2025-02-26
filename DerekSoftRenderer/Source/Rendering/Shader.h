#pragma once
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

class Shader
{
public:
	Shader();
	virtual ~Shader();
	FragmentInput virtual VertexShader(const VertexInput& input);
	Vector3f virtual FragmentShader(const FragmentInput& input);
};

