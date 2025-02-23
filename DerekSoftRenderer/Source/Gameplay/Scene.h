#pragma once

#include <iostream>
#include <vector>

#include "Object.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	Scene(int maxObjectNum);
	~Scene();

public:
	std::vector<Object*> objectArray;
	std::unique_ptr<Camera> camera;
	//std::unique_ptr<Light> mainLight;
public:
	void AddObject(Object* newObj);

	void SetMainCamera(Camera* mainCam);
};

