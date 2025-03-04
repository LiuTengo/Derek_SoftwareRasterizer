#pragma once

#include <iostream>
#include <vector>

#include "MeshObject.h"
#include "Camera.h"
#include "../Rendering/Light.h"

class Renderer;

class Scene
{
public:
	Scene();
	Scene(int maxObjectNum);
	~Scene();

public:
	std::vector<MeshObject*> objectArray;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
public:
	void Update(float tick);
	void AddObject(MeshObject* newObj);
	void SetMainCamera(Camera mainCam);
	void SetMainLight(Light mainLight);
	void SetRenderer(std::shared_ptr<Renderer> rend);
	Matrix4X4f GetVPMatrix();
};

