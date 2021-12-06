#pragma once
#include "Module.h"
#include "Math.h"

class GameObject;
class Model;
class ComponentCamera;
class Scene;

class ModuleSceneManager :
    public Module
{
	public:
		ModuleSceneManager();
		~ModuleSceneManager();

		void LoadModel(const char* _fileName);

	private:
		Scene* main_scene = nullptr;
};

