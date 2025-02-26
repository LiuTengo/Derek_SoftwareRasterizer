#pragma once

#include "../Math/Vector.hpp"
#include <tuple>
#include <array>
#include <algorithm>

struct Vertex {
	Vector4f points;
	Vector3f normal;
	Vector3f color;

	Vertex() {
		points = Vector4f{0,0,0,1};
		normal = Vector3f{ 0,0,0 };
		color = Vector3f{ 0,0,0 };
	}

	Vertex(Vector4f p,Vector3f norm,Vector3f col) {
		points = Vector4f{ 0,0,0,1 };
		normal = Vector3f{ 0,0,0 };
		color = Vector3f{ 0,0,0 };
	}
};

class Triangle
{
public:
	Triangle();

public:
	Vertex vertices[3];

	void SetModelSpacePoints(int index,const Vector4f& pos);
	void SetModelSpacePoints(int index, const Vector3f& pos);
	void SetNormal(int index, const Vector3f& norm);
	void SetPointColor(int index, const Vector3f& col);
	void SetVerticesData(int index, const Vector4f& pos, const Vector3f& norm, const Vector3f& col);

	std::array<Vector4f,3> toVector4() const;
};

