#pragma once

#include <tuple>
#include <array>
#include <algorithm>

#include "../Math/Vector.hpp"
#include "Vertex.hpp"


class Triangle
{
public:
	Triangle();

public:
	Vertex vertices[3];

	void SetModelSpacePoints(int index,const Vector4f& pos);
	void SetModelSpacePoints(int index, const Vector3f& pos);
	void SetNormal(int index, const Vector3f& norm);
	void SetPointColor(int index, const Vector2f& uv);
	void SetVerticesData(int index, const Vector4f& pos, const Vector3f& norm, const Vector2f& uv);
	void SetVertexData(int index,const FragmentInput& fragInput);
	std::array<Vector4f,3> toVector4() const;
};

