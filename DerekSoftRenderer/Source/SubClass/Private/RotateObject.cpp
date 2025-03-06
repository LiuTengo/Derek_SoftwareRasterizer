#include "../RotateObject.h"

RotateObject::RotateObject(Material* mat) :MeshObject(mat)
{
}

RotateObject::RotateObject(const std::string& filePath, Material* mat) :MeshObject(filePath, mat)
{
}

RotateObject::~RotateObject()
{
}

void RotateObject::Update(float dt)
{
	float offsetAngle = dt * 30.0f;
	//rotation.y() = rotation.y() + offsetAngle;
}
