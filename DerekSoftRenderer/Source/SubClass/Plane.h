#pragma once

#include "../Gameplay/MeshObject.h"

class Plane : public MeshObject
{
public:
	Plane(Material* mat);
	Plane(const std::string& filePath, Material* mat);
	virtual ~Plane() override; 

	virtual void Update(float dt) override;
};

