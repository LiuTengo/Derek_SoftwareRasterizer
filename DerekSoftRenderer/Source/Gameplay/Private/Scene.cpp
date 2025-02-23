#include "../Scene.h"

Scene::Scene()
{
	objectArray = std::vector<Object*>(1, nullptr);
}

Scene::Scene(int maxObjectNum)
{
	objectArray = std::vector<Object*>(maxObjectNum,nullptr);
}

Scene::~Scene()
{
}

void Scene::AddObject(Object* newObj)
{
	if (newObj != nullptr) {
		objectArray.push_back(newObj);
		if (typeid(newObj) ==  typeid(Camera)) {
			std::swap(objectArray[0], objectArray[objectArray.size()-1]);
		}
	}
}

void Scene::SetMainCamera(Camera* mainCam)
{
	camera = std::make_unique<Camera>(mainCam);
}
