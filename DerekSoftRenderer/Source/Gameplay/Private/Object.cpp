#include "../Object.h"

Matrix4X4f Object::GetModelMatrix() const
{
	Matrix4X4f translateM = {
		{1,0,0,-position.x()},
		{0,1,0,-position.y()},
		{0,0,1,-position.z()},
		{0,0,0,1}
	};
	Matrix4X4f scaleM = {
		{scale.x(),0,0,0},
		{0,scale.y(),0,0},
		{0,0,scale.z(),0},
		{0,0,0,1}
	};
	float sa = sin(rotation.x());
	float sb = sin(rotation.y());
	float sc = sin(rotation.z());
	float ca = cos(rotation.x());
	float cb = cos(rotation.y());
	float cc = cos(rotation.z());
	//Notie: 可能会出错?
	//Fixme: 使用四元数旋转
	Matrix4X4f rotationM = {
		{cc * cb,cc * sb * sa - sc * ca,sc * sa + cc * sb * ca,0},
		{sc * cb,cc * ca + sb * sc * sa,sc * sb * ca - cc * sa,0},
		{-sb,cb*sa,cb*ca,0},
		{0,0,0,1}
	};

	return translateM * rotationM * scaleM;
}

void Object::SetMaterial(Material* mat)
{
	material.reset(mat);
}

void Object::UseVertexShader()
{
	if (material != nullptr) {
		material->ApplyVertexShader();
	}
	else {
		//Debug
	}
}

Vector3f Object::UseFragmentShader()
{
	if (material != nullptr) {
		return material->ApplyFragmentShader();
	}
	else {
		//Debug
		return Vector3f();
	}
}
