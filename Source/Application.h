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
class ModuleLoadModels;
class ModuleProgram;

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
		inline float GetDeltaTime() { return delta_time; };

	public:
		ModuleRender* renderer = nullptr;
		ModuleWindow* window = nullptr;
		ModuleInput* input = nullptr;
		ModuleGui* gui = nullptr;
		ModuleDebugDraw* debug_draw = nullptr;
		ModuleCamera* camera = nullptr;
		ModuleTexture* texture = nullptr;
		ModuleLoadModels* moduleLoadModels = nullptr;
		ModuleProgram* program = nullptr;

		AppLog* console = nullptr;

	private:
		int	capped_ms;
		std::vector<Module*> modules;

		float delta_time = 0.0f;
		float last_time = 0.0f;
};

extern Application* App;
