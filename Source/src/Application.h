#pragma once

#include "Globals.h"
#include "Modules/Module.h"
#include "Utils/Timer.h"

#include<list>
#include <vector>

using namespace std;

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;
class ModuleSceneManager;
class ModuleProgram;
class ModuleHardware;
class ModuleFileSystem;
class ModuleResourceManager;

class Program;
class AppLog;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();

	bool CleanUp();

	void RequestBrowser(const char* url) const;

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleDebugDraw* debug_draw = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleSceneManager* scene_manager = nullptr;
	ModuleProgram* program = nullptr;
	ModuleHardware* hw = nullptr;
	ModuleFileSystem* file_sys = nullptr;
	ModuleResourceManager* resource_manager = nullptr;

private:
	PerformanceTimer timer;

	double  delta = 0;
	double  prev_tick_time = 0;
	std::vector<Module*> modules;
};

extern Application* App;
