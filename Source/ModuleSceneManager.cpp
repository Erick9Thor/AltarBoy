#include "ModuleSceneManager.h"
#include "Scene.h"

#include "Model.h"

ModuleSceneManager::ModuleSceneManager()
{
	main_scene = new Scene();
}

ModuleSceneManager::~ModuleSceneManager()
{
}

update_status ModuleSceneManager::Update()
{
	main_scene->Update();
	main_scene->DrawScene();

	return UPDATE_CONTINUE;
}

void ModuleSceneManager::LoadModel(const char* _fileName)
{
	// delete scene_model;
	// scene_model = new Model(_fileName);

	// TODO: Need to refactor model class to convert a game object
}