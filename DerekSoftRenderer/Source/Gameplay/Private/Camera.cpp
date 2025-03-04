#include "../Camera.h"

Camera::Camera()
	:nearPlane(0.1f),farPlane(100.0f),fov(45.0f),
	aspectRatio(RendererSettings::WINDOW_WIDTH/ RendererSettings::WINDOW_HEIGHT),
	lookTarget(Vector3f{0,0,0})
{
	position = Vector3f{0,0,8};
	cameraDirection = Vector3f::normalize(lookTarget-position);
	Vector3f up = Vector3f{0,1,0};

	cameraRight = Vector3f::normalize(up.cross(cameraDirection));
	cameraUp = Vector3f::normalize(cameraDirection.cross(cameraRight));
}

Camera::Camera(Vector3f lookPoint,float far, float near, float fov, float eyeRatio)
	:nearPlane(near), farPlane(far), fov(fov), aspectRatio(eyeRatio),lookTarget(lookPoint)
{
	position = Vector3f{ 0,0,1};
	cameraDirection = Vector3f::normalize(lookTarget - position);
	Vector3f up = Vector3f{ 0,1,0 };

	cameraRight = Vector3f::normalize(up.cross(cameraDirection));
	cameraUp = Vector3f::normalize(cameraDirection.cross(cameraRight));
}

Matrix4X4f Camera::GetProjectionMatrix(ProjectionType type) const
{
	//Perspective Matrix
	Matrix4X4f perspectiveM = {
		{nearPlane,0,0,0},
		{0,nearPlane,0,0},
		{0,0,nearPlane + farPlane,-nearPlane * farPlane},
		{0,0,1,0}
	};
	
	float eye_fov = fov * PI/ 180.0f;
	float t = nearPlane * tan(fov*0.5f);
	float b = -t;
	float r = aspectRatio*t;
	float l = -r;
	//Orthographic Matrix
	Matrix4X4f orthographicM = {
		{2/(r-l),0,0,-(r + l) / 2},
		{0,2/(t-b),0,-(t + b) / 2},
		{0,0,2/(nearPlane - farPlane),-(nearPlane + farPlane) / 2},
		{0,0,0,1}
	};

    return orthographicM*perspectiveM;
}

Matrix4X4f Camera::GetViewMatrix() const
{
    Matrix4X4f viewMat = {
        {cameraRight.x(),cameraUp.x(),-cameraDirection.x(),-position.x()},
        {cameraRight.y(),cameraUp.y(),-cameraDirection.y(),-position.y()},
        {cameraRight.z(),cameraUp.z(),-cameraDirection.z(),-position.z()},
        {0,0,0,1}
    };

    return viewMat;
}

Matrix4X4f Camera::GetVPMatrix() const
{
	Matrix4X4f vpMatrix;
	RendererSettings::v = GetViewMatrix();
	vpMatrix = GetProjectionMatrix() * GetViewMatrix();
	return vpMatrix;
}

float Camera::GetFarSubstractNear() const
{
	return farPlane-nearPlane;
}
