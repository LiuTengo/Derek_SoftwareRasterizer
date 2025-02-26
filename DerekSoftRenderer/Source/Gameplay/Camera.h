#pragma once
#include "Object.h"
#include "../Math/Vector.hpp"
#include "../Math/Matrix.hpp"
#include "../Rendering/RendererSettings.h"
#include "../Math/ConstMathValue.hpp"

enum class ProjectionType {
	Orthographic,
	Perspective
};

class Camera : public Object
{
public:
	float farPlane, nearPlane;
	float fov;
	float aspectRatio;

	Vector3f position;
	Vector3f rotation;

public:
	Camera();
	Camera(float far,float near,float fov,float eyeRatio);

public:
	Matrix4X4f GetProjectionMatrix(ProjectionType type = ProjectionType::Perspective) const;
	Matrix4X4f GetViewMatrix() const;
	Matrix4X4f GetVPMatrix() const;
};

