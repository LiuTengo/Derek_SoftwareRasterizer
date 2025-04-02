#include "../Triangle.h"

Triangle::Triangle()
{
    vertices[0] = Vertex();
    vertices[1] = Vertex();
    vertices[2] = Vertex();
}

void Triangle::SetModelSpacePoints(int index, const Vector4f& pos)
{
    vertices[index].clipPoints = pos;
}

void Triangle::SetModelSpacePoints(int index, const Vector3f& pos)
{
    vertices[index].clipPoints = Vector4f{ pos.x(),pos.y() ,pos.z() ,1};
}

void Triangle::SetNormal(int index, const Vector3f& norm)
{
    vertices[index].normal = norm;
}

void Triangle::SetPointColor(int index, const Vector2f& uv)
{
    vertices[index].uv = uv;
}

void Triangle::SetVerticesData(int index, const Vector4f& pos, const Vector3f& norm, const Vector2f& uv)
{
    vertices[index].clipPoints = pos;
    vertices[index].normal = norm;
    vertices[index].uv = uv;
}

void Triangle::SetVertexData(int index, const FragmentInput& fragInput)
{
    vertices[index].clipPoints = fragInput.clipPosition;
    vertices[index].worldPosition = fragInput.worldPosition;
    vertices[index].normal = fragInput.worldNormal;
}

Vector3f Triangle::GetTriangleNormal() const
{
    Vector3f v21 = (vertices[0].clipPoints - vertices[1].clipPoints).toVector3();
    Vector3f v32 = (vertices[2].clipPoints - vertices[1].clipPoints).toVector3();
    return Vector3f::normalize(v21.cross(v32));
}

std::array<Vector4f, 3> Triangle::toVector4() const
{
    std::array<Vector4f, 3> res;
    std::transform(std::begin(vertices), std::end(vertices), res.begin(), [](auto& v) { return Vector4f{ v.clipPoints.x(), v.clipPoints.y(), v.clipPoints.z(), 1 }; });
    return res;
}
