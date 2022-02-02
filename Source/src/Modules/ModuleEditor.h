#pragma once

#include "../Globals.h"
#include "../Application.h"
#include "../GameObject.h"

#include "../UI/Window.h"
#include "../UI/WindowAbout.h"
#include "../UI/WindowConfiguration.h"
#include "../UI/WindowHierarchy.h"
#include "../UI/WindowInspector.h"
#include "../UI/WindowScene.h"
#include "../UI/WindowResource.h"
#include "../UI/WindowProject.h"
#include "../UI/WindowConsole.h"
#include "../UI/WindowTimers.h"

#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor();

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	update_status Update(const float delta) override;
	bool CleanUp() override;

	//Edit actions
	bool CanUndo() { return false; }
	bool CanRedo() { return false; }
	bool CanPaste() { return false; }

	void SetSelectedGO(GameObject* go) { selected_go = go; }
	GameObject* GetSelectedGO() { return selected_go; }
	const GameObject* GetSelectedGO() const { return selected_go; }

	const WindowScene* GetSceneWindow() const { return &w_scene; }

	ImFont* m_big_icon_font = nullptr;
	ImFont* m_small_icon_font = nullptr;

	// TODO: Save the info imgui.ini
	unsigned dock_up_id = 0;
	unsigned dock_main_id = 0;
	unsigned dock_left_id = 0;
	unsigned dock_right_id = 0;
	unsigned dock_down_id = 0;

private:
	void RenderGui();
	void GenerateDockingSpace();

	// Main menu bar
	update_status MainMenuBar();
	void FileMenu();
	void EditMenu();
	void GoMenu();
	void ViewMenu();

	GameObject* selected_go = nullptr;

	std::vector<Window*> windows;

	WindowHierarchy w_hierarchy;
	WindowScene w_scene;
	WindowInspector w_inspector;
	WindowConfiguration w_configuration;
	WindowAbout w_about;
	WindowConsole w_console;
	WindowResource w_resource;
	WindowProject w_project;
	WindowTimers w_timers;
};
