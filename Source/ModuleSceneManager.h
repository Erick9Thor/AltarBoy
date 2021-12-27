#pragma once
#include "Module.h"
#include "Math.h"

class GameObject;
class ComponentCamera;
class Scene;

class ModuleSceneManager :
    public Module
{
	public:
		ModuleSceneManager();
		~ModuleSceneManager() override;

		bool Init() override;
		update_status Update(const float delta) override;
		bool CleanUp() override;

		void DrawScenes();

		void LoadModel(const char* model_path);

		GameObject* GetRoot();
		const GameObject* GetRoot() const;

		Scene* GetActiveScene() {
			return main_scene;
		}

		bool isSceneRuning() { return false; }

	private:
		Scene* main_scene = nullptr;
};

