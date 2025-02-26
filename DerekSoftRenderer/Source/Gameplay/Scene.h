#pragma once

#include <iostream>
#include <vector>

#include "MeshObject.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	Scene(int maxObjectNum);
	~Scene();

public:
	std::vector<MeshObject*> objectArray;
	std::unique_ptr<Camera> camera;
	//std::unique_ptr<Light> mainLight;
public:
	void AddObject(MeshObject* newObj);
	void SetMainCamera(Camera mainCam);
	void SetRenderer(std::shared_ptr<Renderer> rend);
	Matrix4X4f GetVPMatrix();
};

