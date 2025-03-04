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
	//Vector3f rotation;
	Vector3f lookTarget;

	Vector3f cameraDirection;
	Vector3f cameraUp;
	Vector3f cameraRight;

public:
	Camera();
	Camera(Vector3f lookPoint, float far,float near,float fov,float eyeRatio);

public:
	Matrix4X4f GetProjectionMatrix(ProjectionType type = ProjectionType::Perspective) const;
	Matrix4X4f GetViewMatrix() const;
	Matrix4X4f GetVPMatrix() const;
	float GetFarSubstractNear() const;
};

