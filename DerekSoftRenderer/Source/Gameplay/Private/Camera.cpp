#include "../Camera.h"
#include "../../Rendering/RendererSettings.h"
#include "../../Math/ConstMathValue.hpp"

Camera::Camera()
	:nearPlane(0.1f),farPlane(50.0f),fov(45.0f),aspectRatio(RendererSettings::WINDOW_WIDTH/ RendererSettings::WINDOW_HEIGHT)
{
}

Camera::Camera(float far, float near, float fov, float eyeRatio)
	:nearPlane(near), farPlane(far), fov(fov), aspectRatio(eyeRatio)
{
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
    Matrix4X4f translateM = {
        {1,0,0,-position.x()},
        {0,1,0,-position.y()},
        {0,0,1,-position.z()},
        {0,0,0,1}
    };

	float sa = sin(rotation.x());
	float sb = sin(rotation.y());
	float sc = sin(rotation.z());
	float ca = cos(rotation.x());
	float cb = cos(rotation.y());
	float cc = cos(rotation.z());
	//Notie: 可能会出错
	//Fixme: 使用四元数旋转
	Matrix4X4f rotationM = {
		{cc * cb,cc * sb * sa - sc * ca,sc * sa + cc * sb * ca,0},
		{sc * cb,cc * ca + sb * sc * sa,sc * sb * ca - cc * sa,0},
		{-sb,cb * sa,cb * ca,0},
		{0,0,0,1}
	};

	Matrix4X4f rotateMInv = rotationM.Transpose();

    return rotateMInv *translateM;
}

Matrix4X4f Camera::GetVPMatrix() const
{
	Matrix4X4f vpMatrix;
	vpMatrix = GetProjectionMatrix() * GetViewMatrix();
	return vpMatrix;
}
