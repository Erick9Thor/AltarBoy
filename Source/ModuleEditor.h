#pragma once

#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew.h"
#include "GameObject.h"

#include "ImGuiComponents/AppLog.h"

#include <string.h>
using namespace std;

class ModuleEditor : public Module
{
	public:
		ModuleEditor();
		~ModuleEditor() override;

		bool Init() override;
		update_status PreUpdate(const float delta) override;
		update_status Update(const float delta) override;
		bool CleanUp() override;
		
		// Main menu bar
		void FileMenu();
		void EditMenu();
		void GoMenu();
		void ViewMenu();

		//Edit actions
		bool canUndo() { return false; }
		bool canRedo() { return false; }
		bool canPaste() { return false; }
		
		void Draw();
		void showWindowsViewports();

		GameObject* getSelectedGO() const { return selected_go; }

		// HIERARCHY
		void DrawHierarchyTree(GameObject* root);
		void DrawGOChilds(GameObject* root);
		void DrawGameObject(GameObject* go);
		void ShowGameObject(GameObject* go);

		// INSPECTOR
		void InspectorDrawGameObject(GameObject* go);
	
		void showAbaoutInfo();

	private:

		GameObject* selected_go = nullptr;

		bool show_hirarchy = true;
		bool show_inspector_window = true;
		bool show_abaout = false;
		bool show_camera_window = false;
		bool show_fps_counter = false;
		bool show_asset_browser = false;
};
