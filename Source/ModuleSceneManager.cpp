#include "ModuleSceneManager.h"
#include "Scene.h"

ModuleSceneManager::ModuleSceneManager()
{
	main_scene = new Scene();
}

ModuleSceneManager::~ModuleSceneManager()
{
}


void ModuleSceneManager::LoadModel(const char* _fileName)
{
}
