#include "ModuleSceneManager.h"

#include "../Importers/SceneImporter.h"
#include "../Utils/Logger.h"

ModuleSceneManager::ModuleSceneManager() {}

ModuleSceneManager::~ModuleSceneManager() {}

bool ModuleSceneManager::Init()
{
	LOG("Creating Empty scene");

	// main_scene = new Scene();

	main_scene = SceneImporter::LoadScene(ASSETS_FOLDER "/Scenes/lights_delivery.scene");
	
	//LoadModel(ASSETS_FOLDER "\\Models\\BakerHouse.fbx"); //TODO: Remove this when importen will be created

	// CreateEmptyScene();
	// LoadScene(LIBRARY_SCENE_FOLDER "/survival_shooter.scene");
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
	main_scene->LoadFBX(model_path);
}

void ModuleSceneManager::CreateEmptyScene()
{
	delete main_scene;
	main_scene = new Scene();
}

void ModuleSceneManager::LoadScene(const char* file_path)
{
	delete main_scene;
	main_scene = SceneImporter::LoadScene(file_path);
}

void ModuleSceneManager::SaveScene(const char* file_path)
{
	SceneImporter::SaveScene(main_scene, file_path);
}
