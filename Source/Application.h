#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include <vector>
#include "Timer.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;
class ModuleLoadModels;
class ModuleProgram;
class ModuleHardware;

class Program;
class AppLog;

class Application
{
	public:

		Application();
		~Application();

		bool Init();
		update_status Update();

		void FinishUpdate();
		bool CleanUp();

		void DebugDraw();

		void RequestBrowser(const char* url) const;
		inline float GetDeltaTime() { return delta_time; };

		int GetFramerateLimit() const;
		void SetFramerateLimit(int max_framerate);

	public:
		ModuleRender* renderer = nullptr;
		ModuleWindow* window = nullptr;
		ModuleInput* input = nullptr;
		ModuleEditor* editor = nullptr;
		ModuleDebugDraw* debug_draw = nullptr;
		ModuleCamera* camera = nullptr;
		ModuleTexture* texture = nullptr;
		ModuleLoadModels* moduleLoadModels = nullptr;
		ModuleProgram* program = nullptr;
		ModuleHardware* hw = nullptr;

		AppLog* console = nullptr;

	private:

		Timer ms_timer;
		Timer fps_timer;

		int frames;
		int	capped_ms;
		int	fps_counter;
		int	last_frame_ms;
		int	last_fps;
		float delta_time = 0.0f;

		std::vector<Module*> modules;
};

extern Application* App;
