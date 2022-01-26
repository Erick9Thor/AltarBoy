#pragma once
#include "Module.h"
#include "Math.h"

#include "../Scene.h"

class GameObject;
class ComponentCamera;

class ModuleSceneManager : public Module
{
public:
	ModuleSceneManager();
	~ModuleSceneManager() override;

	// --- Life cycle --- //
	bool Init() override;
	update_status Update(const float delta) override;
	bool CleanUp() override;

	// --- Scene management --- //
	void LoadModel(const char* model_path); // TODO: delete Change to load scene and load al gameObjects for path

	GameObject* GetRoot() { return main_scene->root; }
	const GameObject* GetRoot() const { return main_scene->root; }
	Scene* GetActiveScene() { return main_scene; }
	const Scene* GetActiveScene() const { return main_scene; }

	void CreateEmptyScene();

	void LoadScene(const char* file_path = "untitled");
	void SaveScene(const char* file_path);

private:
	Scene* main_scene = nullptr;
};
