#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include <vector>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleGui;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;
class ModuleRenderExercise;

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

	void DebugDraw();

	void RequestBrowser(const char* url) const;
	int GetFramerateLimit() const;

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleGui* gui = nullptr;
	ModuleDebugDraw* debug_draw = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleRenderExercise* rendererExercise = nullptr;

	AppLog* console = nullptr;

	// TODO: Convert this on a utils provider or something like
	Program* program = nullptr;

private:
	int	capped_ms;
	std::vector<Module*> modules;
};

extern Application* App;
