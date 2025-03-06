#include "../Plane.h"

Plane::Plane(Material* mat):MeshObject(mat)
{
}

Plane::Plane(const std::string& filePath, Material* mat):MeshObject(filePath,mat)
{
}

Plane::~Plane()
{
}

void Plane::Update(float dt)
{

}
