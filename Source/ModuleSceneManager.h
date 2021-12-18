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
		~ModuleSceneManager() override;

		update_status Update(const float delta) override;

		void LoadModel(const char* _fileName);

		GameObject* GetRoot();
		const GameObject* GetRoot() const;

		Model* scene_model;

		Scene* getActiveScene() {
			return main_scene;
		}

	private:
		Scene* main_scene = nullptr;
};

