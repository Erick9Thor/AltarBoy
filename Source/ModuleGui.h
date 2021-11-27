#pragma once
#ifndef __MODULE_GUI_H__
#define __MODULE_GUI_H__

#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew.h"

#include "ImGuiComponents/AppLog.h"


class ModuleGui : public Module
{
public:
	ModuleGui() :fps_log(100), ms_log(100) {};
	~ModuleGui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Draw();

	void showMenu();
	
	void showFPSGraph();
	void showConfigWindow();
	void showHardwareInfo();
	void showAbaoutInfo();

private:
	std::vector<float> ms_log;
	std::vector<float> fps_log;

	bool show_camera_window = false;
	bool show_texture_window = false;
	bool show_model_window = false;
	bool fps_counter = false;
};

#endif // MODULE_GUI_H