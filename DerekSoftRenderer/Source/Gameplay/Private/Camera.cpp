#include "../Camera.h"

Camera::Camera()
	:nearPlane(0.1f),farPlane(50.0f),fov(60.0f),
	aspectRatio(RendererSettings::WINDOW_WIDTH/ RendererSettings::WINDOW_HEIGHT),
	lookTarget(Vector3f{0,0,0})
{
	position = Vector3f{0,0,5};
	cameraDirection = Vector3f::normalize(lookTarget-position);
	Vector3f up = Vector3f{ 0,1,0 };

	cameraRight = Vector3f::normalize(cameraDirection.cross(up));
	cameraUp = Vector3f::normalize(cameraRight.cross(cameraDirection));
}

Camera::Camera(const Vector3f& position, const Vector3f& lookPoint,float far, float near, float fov, float eyeRatio)
	:position(position),nearPlane(near), farPlane(far), fov(fov), aspectRatio(eyeRatio),lookTarget(lookPoint)
{
	cameraDirection = Vector3f::normalize(lookTarget - position);
	Vector3f up = Vector3f{ 0,1,0 };

	cameraRight = Vector3f::normalize(cameraDirection.cross(up));
	cameraUp = Vector3f::normalize(cameraRight.cross(cameraDirection));
}

Matrix4X4f Camera::GetProjectionMatrix(ProjectionType type) const
{
	float tanHalfFov = tan(fov * 0.5f * PI / 180.0f); // fov 转换为弧度
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
        {cameraRight.x(),cameraRight.y(),cameraRight.z() ,0},
        {cameraUp.x(),cameraUp.y(),cameraUp.z() ,0},
        {-cameraDirection.x(),-cameraDirection.y(),-cameraDirection.z(),0},
        {0,0,0,1}
    };

	Matrix4X4f transMat = {
		{1,0,0,-position.x()},
		{0,1,0,-position.y()},
		{0,0,1,-position.z()},
		{0,0,0,1},
	};

    return viewMat*transMat;
}

Matrix4X4f Camera::GetVPMatrix() const
{
	Matrix4X4f vpMatrix;
	vpMatrix = GetProjectionMatrix() * GetViewMatrix();
	return vpMatrix;
}

float Camera::GetFarSubstractNear() const
{
	return farPlane-nearPlane;
}

float Camera::GetNearPlane() const
{
	return nearPlane;
}

/// <summary>
/// 非线性深度参数
/// </summary>
/// <returns></returns>
double Camera::GetNonLinearDepth(double z) const
{
	double inv_z = 1. / z;
	return inv_z*((nearPlane- z)*farPlane)/(nearPlane-farPlane);
	//float f1 = (farPlane - nearPlane) / 2;
	////float f2 = (farPlane + nearPlane) / 2;
	//return  z * f1 + f2;
}

double Camera::GetLinearDepth(double z) const
{
	return ((nearPlane- z)/(nearPlane - farPlane));
}

