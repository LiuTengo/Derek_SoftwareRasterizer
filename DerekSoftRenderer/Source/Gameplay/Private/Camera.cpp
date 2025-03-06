#include "../Camera.h"

Camera::Camera()
	:nearPlane(0.1f),farPlane(100.0f),fov(45.0f),
	aspectRatio(RendererSettings::WINDOW_WIDTH/ RendererSettings::WINDOW_HEIGHT),
	lookTarget(Vector3f{0,0,0})
{
	position = Vector3f{0,0,5};
	cameraDirection = Vector3f::normalize(lookTarget-position);
	Vector3f up = Vector3f{0,1,0};

	cameraRight = Vector3f::normalize(cameraDirection.cross(up));
	cameraUp = Vector3f::normalize(cameraRight.cross(cameraDirection));
}

Camera::Camera(Vector3f lookPoint,float far, float near, float fov, float eyeRatio)
	:nearPlane(near), farPlane(far), fov(fov), aspectRatio(eyeRatio),lookTarget(lookPoint)
{
	position = Vector3f{ 0,0,1};
	cameraDirection = Vector3f::normalize(lookTarget - position);
	Vector3f up = Vector3f{ 0,1,0 };

	cameraRight = Vector3f::normalize(cameraDirection.cross(up));
	cameraUp = Vector3f::normalize(cameraRight.cross(cameraDirection));
}

Matrix4X4f Camera::GetProjectionMatrix(ProjectionType type) const
{
	float tanHalfFov = tan(fov * 0.5f * PI / 180.0f); // fov 应该转换为弧度
	Matrix4X4f perspectiveM = {
		{1.0f / (aspectRatio * tanHalfFov), 0, 0, 0},
		{0, 1.0f / tanHalfFov, 0, 0},
		{0, 0, -(farPlane + nearPlane) / (farPlane - nearPlane), -2.0f * farPlane * nearPlane / (farPlane - nearPlane)},
		{0, 0, -1, 0}
	};
    return perspectiveM;
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
