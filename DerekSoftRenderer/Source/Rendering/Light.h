#pragma once

#include "../Math/Vector.hpp"

class Light
{
public:
	Light();

public:
	Vector4f Position;
	Vector3f Direction;
	Vector3f Color;
	float Intensity;
};

