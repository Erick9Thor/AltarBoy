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
	int GetFramerateLimit() const;

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleGui* gui = nullptr;

	AppLog* console = nullptr;

private:
	int	capped_ms;
	std::vector<Module*> modules;

};

extern Application* App;
