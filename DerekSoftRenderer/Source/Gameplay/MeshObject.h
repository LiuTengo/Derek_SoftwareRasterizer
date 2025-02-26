#pragma once
#include <windows.h>
#include <vector>

#include "Object.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/Material.h"
#include "../Math/Matrix.hpp"
#include "../ObjLoader/OBJ_Loader.h"

class MeshObject :public Object
{
public:
	MeshObject() = default;
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
	std::weak_ptr<Renderer> renderer;
public:
	Matrix4X4f GetModelMatrix() const;
	void SetMaterial(Material* mat);
	void UseVertexShader();
	Vector3f UseFragmentShader();
	void SetMVPMatrix();
	void SetRenderer(std::shared_ptr<Renderer> r);
private:
	void SetupShaderInputData();
};