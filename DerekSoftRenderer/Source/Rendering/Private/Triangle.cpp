#include "../Triangle.h"

Triangle::Triangle()
{
    points[0] = Vector4f{ 0,0,0,1 };
    points[1] = Vector4f{ 0,0,0,1 };
    points[2] = Vector4f{ 0,0,0,1 };

    normal[0] = Vector3f();
    normal[1] = Vector3f();
    normal[2] = Vector3f();

    color[0] = Vector3f();
    color[1] = Vector3f();
    color[2] = Vector3f();
}

void Triangle::SetModelSpacePoints(int index, const Vector4f& pos)
{
    points[index] = pos;
}

void Triangle::SetNormal(int index, const Vector3f& norm)
{
    normal[index] = norm;
}

void Triangle::SetPointColor(int index, const Vector3f& col)
{
    color[index] = col;
}

std::array<Vector4f, 3> Triangle::toVector4() const
{
    std::array<Vector4f, 3> res;
    std::transform(std::begin(points), std::end(points), res.begin(), [](auto& vec) { return Vector4f{ vec.x(), vec.y(), vec.z(), 1 }; });
    return res;
}
