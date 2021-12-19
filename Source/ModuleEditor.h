#pragma once

#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew.h"
#include "GameObject.h"

#include "ImGuiComponents/AppLog.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor() override;

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	update_status Update(const float delta) override;
	bool CleanUp() override;

	void DrawViewport();
	void DrawScene();

	void Draw();

	void showMenu();

	void DrawHierarchyTree(GameObject* root);
	void DrawGOChilds(GameObject* root);
	void DrawGameObject(GameObject* go);
	void ShowGameObject(GameObject* go);
	
	void showAbaoutInfo();

private:

	bool show_hirarchy = true;
	bool show_inspector_window = true;
	bool show_abaout = false;
	bool show_camera_window = false;
	bool show_fps_counter = false;
};
