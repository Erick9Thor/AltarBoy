#pragma once

#include "Geometry/LineSegment.h"
#include "Modules/ModuleTexture.h"
#include "Components/Component.h"
#include "Resources/ResourceMaterial.h"

#include "assimp/scene.h"

#include <string>
#include <vector>

class ModuleSceneManager;
class GameObject;
class ComponentTransform;
class ComponentCamera;
class ComponentDirLight;
class ComponentPointLight;
class ComponentSpotLight;
class Skybox;
class Quadtree;

class Scene
{
	friend class ModuleSceneManager;

public:
	Scene();
	~Scene();

	void CleanScene();

	// --- Life cycle Scene --- //
	void Update();
	void Save(JsonFormaterValue j_scene) const;
	void Load(JsonFormaterValue j_scene);

	// --- GameObject Management --- //
	void AddGameObject(GameObject* new_object, GameObject* parent = nullptr);
	void DestroyGameObject(GameObject* game_object);
	GameObject* CreateNewGameObject(const char* name, GameObject* parent = nullptr);
	GameObject* GetRoot() const { return root; }

	// --- Quadtree --- //
	Quadtree* GetQuadtree() const { return quadtree; }

	// --- Debug --- //
	GameObject* CreateDebugCamera();

	// --- Skybox --- //
	Skybox* GetSkybox() const { return skybox; }
	
	GameObject* RayCast(const LineSegment& segment) const;

	void CreateLights();
	std::vector<ComponentDirLight*> dir_lights;
	std::vector<ComponentPointLight*> point_lights;
	std::vector<ComponentSpotLight*> spot_lights;

	// --- Importer --- // TODO: Move to importer
	GameObject* LoadFBX(const std::string& path);

private:
	// TODO: Fix leaks from resource material pointers when doing import system
	void LoadNode(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& model_path);
	//std::vector<ResourceMaterial*> LoadMaterials(const aiScene* scene, const std::string& model_path, const std::string& model_name);

private:
	GameObject* root = nullptr;

	Skybox* skybox;
	Quadtree* quadtree = nullptr;

	bool draw_all_bounding_boxes = false;
};
