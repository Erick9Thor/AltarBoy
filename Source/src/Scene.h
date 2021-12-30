#pragma once

#include "Math.h"
#include "Modules/ModuleTexture.h"
#include "Components/Component.h"

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
	void LoadNode(const aiScene* scene, const aiNode* node, GameObject* parent, std::vector<Texture>& textures);
	std::vector<Texture> LoadTextures(const aiScene* scene, const std::string& model_path);
	Texture LoadTexture(const aiMaterial* material, const std::string& model_path);

	ModuleSceneManager* manager_owner = nullptr;

	GameObject* root = nullptr;
	ComponentCamera* main_camera = nullptr;
};