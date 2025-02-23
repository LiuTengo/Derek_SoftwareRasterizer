#pragma once
#include <cmath>
#include <vector>
#include "../Rendering/Material.h"
#include "../Math/Vector.hpp"
#include "../Math/Matrix.hpp"

class Object
{
public:
	Object() = default;
	virtual ~Object() = default;

public:
	unsigned int GetID() const {
		return id;
	};
	Matrix4X4f GetModelMatrix() const;
	void SetMaterial(Material* mat);
	void UseVertexShader();
	Vector3f UseFragmentShader();
public:
	std::vector<Triangle> triangleList;

protected:
	unsigned int id;

	//Transform
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	//Rendering
	std::shared_ptr<Material> material;
};

