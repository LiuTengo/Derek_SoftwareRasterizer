#pragma once

#include "../Math/Vector.hpp"
#include <tuple>
#include <array>
#include <algorithm>

class Triangle
{
public:
	Triangle();

public:
	static std::tuple<float, float, float> ComputeBarycentricCoordinate(float x,float y, const Vector3f* v);
public:
	Vector4f points[3];
	Vector3f normal[3];
	Vector3f color[3];

	void SetModelSpacePoints(int index,const Vector4f& pos);
	void SetNormal(int index, const Vector3f& norm);
	void SetPointColor(int index, const Vector3f& col);

	std::array<Vector4f,3> toVector4() const;
};

