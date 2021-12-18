#pragma once

#include "Math.h"
#include "ModuleTexture.h"
#include "Component.h"

#include "assimp/scene.h"

#include <string>
#include <vector>

class ModuleSceneManager;
class GameObject;
class ComponentCamera;

class Scene
{
	friend class ModuleSceneManager;

	public: 
		Scene();
		~Scene();

		void AddGameObject(GameObject* new_object, GameObject* parent = nullptr);
		GameObject* CreateNewGameObject(const char* name, GameObject* parent = nullptr);
		GameObject* LoadFBX(const std::string& path);

		GameObject* CreateCamera();
		const ComponentCamera* GetMainCamera() const;

		void Update();
		
		void Play();
		void Stop();
		void Draw();

	private:
		bool LoadTextures(const aiScene* scene, const std::string& model_path);
		bool LoadTexture(const aiMaterial* texture, const std::string& model_path);		

		ModuleSceneManager* manager_owner = nullptr;

		GameObject* root = nullptr;
		std::vector<Texture> textures;
		ComponentCamera* main_camera = nullptr;
};

