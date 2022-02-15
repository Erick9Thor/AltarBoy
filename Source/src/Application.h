#pragma once

#include "Globals.h"
#include "modules/Module.h"
#include "utils/Timer.h"

#include <vector>

namespace Hachiko
{
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

    class Program;
    class AppLog;

    class Application
    {
    public:
        Application();
        ~Application();

        bool Init();
        UpdateStatus Update();

        bool CleanUp();

        static void RequestBrowser(const char* url);

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

    private:
        PerformanceTimer timer;

        double delta = 0;
        double prev_tick_time = 0;
        std::vector<Module*> modules;
    };
}

extern Hachiko::Application* App;
