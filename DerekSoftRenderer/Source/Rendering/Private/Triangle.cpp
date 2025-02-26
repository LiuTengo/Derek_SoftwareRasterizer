#include "../Triangle.h"

Triangle::Triangle()
{
    vertices[0] = Vertex();
    vertices[1] = Vertex();
    vertices[2] = Vertex();
}

void Triangle::SetModelSpacePoints(int index, const Vector4f& pos)
{
    vertices[index].points = pos;
}

void Triangle::SetModelSpacePoints(int index, const Vector3f& pos)
{
    vertices[index].points = Vector4f{ pos.x(),pos.y() ,pos.z() ,1};
}

void Triangle::SetNormal(int index, const Vector3f& norm)
{
    vertices[index].normal = norm;
}

void Triangle::SetPointColor(int index, const Vector3f& col)
{
    vertices[index].color = col;
}

void Triangle::SetVerticesData(int index, const Vector4f& pos, const Vector3f& norm, const Vector3f& col)
{
    vertices[index].points = pos;
    vertices[index].normal = norm;
    vertices[index].color = col;
}

std::array<Vector4f, 3> Triangle::toVector4() const
{
    std::array<Vector4f, 3> res;
    std::transform(std::begin(vertices), std::end(vertices), res.begin(), [](auto& v) { return Vector4f{ v.points.x(), v.points.y(), v.points.z(), 1 }; });
    return res;
}
