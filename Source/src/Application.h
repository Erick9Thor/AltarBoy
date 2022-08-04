#pragma once

#include "Globals.h"
#include "modules/Module.h"
#include "utils/Timer.h"
#include "utils/FileSystem.h"

#include <vector>

namespace Hachiko
{
    class ModuleEvent;
    class ModuleRender;
    class ModuleWindow;
    class ModuleTextures;
    class ModuleInput;
    class ModuleEditor;
    class ModuleDebugDraw;
    class ModuleCamera;
    class ModuleTexture;
    class ModuleSceneManager;
    class ModuleProgram;
    class ModuleScriptingSystem;
    class ModuleResources;
    class ModuleImporter;
    class ModuleUserInterface;
    class ModuleDebugMode;
    class ModuleAudio;
    class ModuleNavigation;
    class PreferenceManager;
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
        ModuleEvent* event = nullptr;
        ModuleScriptingSystem* scripting_system = nullptr;
        ModuleResources* resources = nullptr;
        ModuleUserInterface* ui = nullptr;
        ModuleDebugMode* debug_mode = nullptr;
        ModuleAudio* audio = nullptr;
        ModuleNavigation* navigation = nullptr;

        PreferenceManager* preferences = nullptr;
        FileSystem file_system;

        [[nodiscard]] bool IsLoading() const;
        void StartLoading() 
        {
            loading = true;
        }
        void LoadingComplete()
        {
            loading = false;
        }

    private:
        PerformanceTimer timer;

        double delta = 0;
        double prev_tick_time = 0;
        std::vector<Module*> modules;

        bool loading = false;
    };
}

extern Hachiko::Application* App;