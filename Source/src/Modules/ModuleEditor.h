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

#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew.h"

#include "../UI/AppLog.h"

class ModuleEditor : public Module
{
public:

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	update_status Update(const float delta) override;
	void RenderGui();
	bool CleanUp() override;

	void GenerateDockingSpace();

	// Main menu bar
	update_status MainMenuBar();
	void FileMenu();
	void EditMenu();
	void GoMenu();
	void ViewMenu();

	//Edit actions
	bool canUndo()
	{
		return false;
	}
	bool canRedo()
	{
		return false;
	}
	bool canPaste()
	{
		return false;
	}

	void setSelectedGO(GameObject* go)
	{
		assert(go);
		selected_go = go;
	}
	GameObject* getSelectedGO() const
	{
		return selected_go;
	}

	ImFont* m_big_icon_font;
	ImFont* m_small_icon_font;

private:
	GameObject* selected_go = nullptr;
	
	std::vector<Window*> windows;

	WindowHierarchy w_hierarchy;
	WindowScene w_scene;
	WindowInspector w_inspector;
	WindowConfiguration w_configuration;
	WindowAbout w_about;
};
