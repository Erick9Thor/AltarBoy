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
	delete root;
	delete main_camera;
}

void Scene::AddGameObject(GameObject* new_object, GameObject* parent)
{

}

GameObject* Scene::CreateNewGameObject(const char* name, GameObject* parent)
{
	GameObject* foo = new GameObject(parent ? parent : root, name);
	foo->scene_owner = this;
	return foo;
}

GameObject* Scene::CreateCamera()
{
	GameObject* camera = CreateNewGameObject("Camera", root);
	camera->GetComponent<ComponentTransform>()->SetLocalPosition(float3(10, 10, 0));
	camera->CreateComponent(Component::Type::Camera);
	camera->GetComponent<ComponentTransform>()->LookAt(float3(0, 5, 0));

	main_camera = camera->GetComponent<ComponentCamera>();

	return camera;
}

const ComponentCamera* Scene::GetMainCamera() const
{
	return main_camera;
}

void Scene::Play()
{
}

void Scene::Stop()
{
}

void Scene::Update()
{
	// root->Update();
}

void Scene::Draw()
{

}
