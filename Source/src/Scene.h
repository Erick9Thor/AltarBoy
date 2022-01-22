#pragma once

#include "Geometry/LineSegment.h"
#include "Modules/ModuleTexture.h"
#include "Components/Component.h"
#include "assimp/scene.h"

#include <string>
#include <vector>

class ModuleSceneManager;
class GameObject;
class ComponentCamera;
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
	ComponentCamera* GetDebugCamera() const { return debug_camera; }

	// --- Skybox --- //
	Skybox* GetSkybox() const { return skybox; }
	
	GameObject* RayCast(const LineSegment& segment) const;

	bool draw_skybox = true;
	std::vector<ComponentPointLight*> point_lights;
	std::vector<ComponentSpotLight*> spot_lights;

	// --- Scene ---// TODO: Move this into WindowConfiguration menu on Scene subsection
	void OptionsMenu();

	// --- Importer --- // TODO: Move to importer
	GameObject* LoadFBX(const std::string& path);
private:
	void LoadNode(const aiScene* scene, const aiNode* node, GameObject* parent, std::vector<Texture>& textures);
	std::vector<Texture> LoadTextures(const aiScene* scene, const std::string& model_path);
	Texture LoadTexture(const aiMaterial* material, const std::string& model_path);

private:
	ModuleSceneManager* manager_owner = nullptr;

	GameObject* root = nullptr;
	ComponentCamera* debug_camera = nullptr;

	Skybox* skybox;
	Quadtree* quadtree = nullptr;

	bool draw_all_bounding_boxes = false;
};
