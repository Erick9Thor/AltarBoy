#pragma once
#include "Module.h"
#include "Math.h"

class GameObject;
class Model;
class ComponentCamera;
class Scene;
class Model;

class ModuleSceneManager :
    public Module
{
	public:
		ModuleSceneManager();
		~ModuleSceneManager();

		update_status Update() override;

		void LoadModel(const char* _fileName);

		GameObject* GetRoot();
		const GameObject* GetRoot() const;

		Model* scene_model;

	private:
		Scene* main_scene = nullptr;
};

