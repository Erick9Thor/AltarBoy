#include "ModuleSceneManager.h"

ModuleSceneManager::ModuleSceneManager() {}

ModuleSceneManager::~ModuleSceneManager() {}

bool ModuleSceneManager::Init()
{
	main_scene = new Scene();
	
	LoadModel("BakerHouse.fbx"); //TODO: Remove this when importen will be created

	CreateEmptyScene();

	return true;
}

update_status ModuleSceneManager::Update(const float delta)
{
	main_scene->Update();
	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{
	RELEASE(main_scene);
	return true;
}

void ModuleSceneManager::LoadModel(const char* model_path)
{
	// delete scene_model;
	// TODO: Need to refactor model class to convert a game object
	main_scene->LoadFBX(model_path);
}

void ModuleSceneManager::CreateEmptyScene()
{


}

void ModuleSceneManager::LoadScene(const char* file_path)
{
}

void ModuleSceneManager::SaveScene(const char* file_path)
{
}
