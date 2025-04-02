#include "../Scene.h"

Scene::Scene()
{
	objectArray = std::vector<MeshObject *>(0, nullptr);
}

Scene::Scene(int maxObjectNum)
{
	objectArray = std::vector<MeshObject *>(maxObjectNum,nullptr);
}

Scene::~Scene()
{
	for (auto& objPtr : objectArray) {
		delete objPtr;
		objPtr = nullptr;
	}
}

void Scene::Update(float tick)
{
	for (const auto& obj : objectArray) {
		obj->Update(tick);
	}
}

void Scene::AddObject(MeshObject* newObj)
{
	if (newObj != nullptr) {
		objectArray.push_back(newObj);
	}
}

void Scene::SetMainCamera(Camera mainCam)
{
	camera = std::make_unique<Camera>(mainCam);
	RendererSettings::CameraPosition = camera->position;
}

void Scene::SetMainLight(Light mainLight)
{
	light = std::make_unique<Light>(mainLight);
	RendererSettings::LightColor = light->Color;
	RendererSettings::LightPosition = light->Position;
	RendererSettings::LightIntensity = light->Intensity;
}

Matrix4X4f Scene::GetVPMatrix()
{
	if (camera) {
		return camera->GetVPMatrix();
	}
	return Matrix4X4f::Identity();
}
