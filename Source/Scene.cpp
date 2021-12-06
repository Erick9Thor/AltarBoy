#include "Scene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Component.h"

Scene::Scene()
{
	root = new GameObject(nullptr, float4x4::identity, "Root");
}

Scene::~Scene()
{
}

void Scene::AddGameObject(GameObject* newGameObject, GameObject* parent)
{

}

GameObject* Scene::CreateNewGameObject(const char* name, GameObject* parent)
{
	return nullptr;
}

GameObject* Scene::CreateCamera()
{
	GameObject* camera = CreateNewGameObject("Camera", root);
	camera->GetComponent<ComponentTransform>()->SetLocalPosition(float3(10, 10, 0));
	camera->CreateComponent(Component::Type::Camera);
	camera->GetComponent<ComponentTransform>()->LookAt(float3(0, 5, 0));
	return nullptr;
}

const ComponentCamera* Scene::GetMainCamera() const
{
	return nullptr;
}

void Scene::Play()
{
}

void Scene::Stop()
{
}
