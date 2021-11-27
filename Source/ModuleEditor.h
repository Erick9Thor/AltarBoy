#pragma once

#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew.h"

#include "ImGuiComponents/AppLog.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor() :fps_log(100), ms_log(100) {};
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Draw();

	void showMenu();
	
	void showFPSGraph();
	void AddFPS(float fps, float ms);

	void showConfigWindow();
	void showHardwareInfo();
	void showAbaoutInfo();

private:
	std::vector<float> ms_log;
	std::vector<float> fps_log;
};
