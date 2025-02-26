#include "../Scene.h"

Scene::Scene()
{
	objectArray = std::vector<MeshObject *>(1, nullptr);
}

Scene::Scene(int maxObjectNum)
{
	objectArray = std::vector<MeshObject *>(maxObjectNum,nullptr);
}

Scene::~Scene()
{
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
}

void Scene::SetRenderer(std::shared_ptr<Renderer> rend)
{
	for (auto& obj : objectArray) {
		obj->SetRenderer(rend);
	}
}

Matrix4X4f Scene::GetVPMatrix()
{
	if (camera) {
		return camera->GetVPMatrix();
	}
	return Matrix4X4f::Identity();
}
