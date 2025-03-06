#pragma once

#include "../Gameplay/MeshObject.h"

class RotateObject :public MeshObject
{
public:
	RotateObject(Material* mat);
	RotateObject(const std::string& filePath, Material* mat);
	virtual ~RotateObject() override;

	virtual void Update(float dt) override;
};

