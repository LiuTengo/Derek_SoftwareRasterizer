#pragma once
#include "Object.h"

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

public:
	Camera();
	Camera(float far,float near,float fov,float eyeRatio);

public:
	Matrix4X4f GetProjectionMatrix(ProjectionType type = ProjectionType::Perspective) const;
	Matrix4X4f GetViewMatrix() const;
	Matrix4X4f GetVPMatrix() const;
};

