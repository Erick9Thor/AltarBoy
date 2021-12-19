#include "ModuleSceneManager.h"
#include "Scene.h"

ModuleSceneManager::ModuleSceneManager()
{
	
}

ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Init()
{
	main_scene = new Scene();
	LoadModel("BakerHouse.fbx");
	return true;
}

update_status ModuleSceneManager::Update(const float delta)
{
	main_scene->Update();
	main_scene->Draw();

	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{
	//TODO: Implement properly
	delete main_scene;
	return true;
}

void ModuleSceneManager::LoadModel(const char* model_path)
{
	// delete scene_model;
	// TODO: Need to refactor model class to convert a game object
	scene_model = main_scene->LoadFBX(model_path);
}

GameObject* ModuleSceneManager::GetRoot()
{
	return main_scene->root;
}

const GameObject* ModuleSceneManager::GetRoot() const
{
	return main_scene->root;
}
