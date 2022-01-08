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

	bool Init() override;
	update_status Update(const float delta) override;
	bool CleanUp() override;

	void LoadModel(const char* model_path);

	GameObject* GetRoot() { return main_scene->root; }
	const GameObject* GetRoot() const { return main_scene->root; }
	Scene* GetActiveScene() { return main_scene; }
	const Scene* GetActiveScene() const { return main_scene; }

	// TODO
	bool IsSceneRuning() { return false; }

private:
	Scene* main_scene = nullptr;
};
