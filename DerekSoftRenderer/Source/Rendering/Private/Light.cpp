#include "../Light.h"

Light::Light()
{
	Position = Vector4f{ 100, 200, 100, 1 };
	Direction = Vector3f::normalize(Vector3f{ -1, -2, -1 });
	Color = Vector3f{1,1,1};
	Intensity = 3.0f;
}
