#pragma once

#include "Math.h"
#include "Component.h"

class ModuleSceneManager;
class GameObject;
class ComponentCamera;

class Scene
{
	friend class ModuleSceneManager;

	public: 
		Scene();
		~Scene();

		void AddGameObject(GameObject* newGameObject, GameObject* parent = nullptr);
		GameObject* CreateNewGameObject(const char* name, GameObject* parent = nullptr);

		GameObject* CreateCamera(); 
		const ComponentCamera* GetMainCamera() const;

		void Play();
		void Stop();

	private:

		void Update();
		void DrawScene();

		ModuleSceneManager* manager_owner = nullptr;

		GameObject* root = nullptr;
		ComponentCamera* main_camera = nullptr;
};

