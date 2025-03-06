#pragma once
#include <windows.h>
#include <vector>

#include "../ObjLoader/OBJ_Loader.h"

#include "../Math/Vector.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/ConstMathValue.hpp"
#include "../Rendering/Shader.h"
#include "../Rendering/Material.h"
#include "Object.h"


class MeshObject :public Object
{
public:
	MeshObject(Material* mat = new Material());
	MeshObject(const std::string& filePath, Material* mat);
	virtual ~MeshObject() override;

public:
	//Transform
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	//Rendering
	std::shared_ptr<Material> material;
	std::vector<Triangle*> triangleList;
	//std::weak_ptr<Renderer> renderer;
public:
	virtual void Update(float dt);

	void SetLocation(const Vector3f& newPos);
	void SetRotation(const Vector3f& newRotation);
	void SetScale(const Vector3f& newScale);
	void Translate(const Vector3f& offset);

	Matrix4X4f GetModelMatrix() const;
	void SetMaterial(Material* mat);
	FragmentInput UseVertexShaderProgram(const VertexInput& vertInput);
	Vector3f UseFragmentShaderProgram(FragmentInput& fragInput);
	void SetMVPMatrix();
private:
	Matrix4X4f vp;
	Matrix4X4f mvp;

private:
	void SetupShaderInputData();
	void LoadModelFromObj(const std::string& filePath = "./Content/Model/box_stack.obj");
};