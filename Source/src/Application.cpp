#include "core/hepch.h"

#include "Application.h"

#include "modules/ModuleWindow.h"
#include "modules/ModuleRender.h"
#include "modules/ModuleInput.h"
#include "modules/ModuleEditor.h"
#include "modules/ModuleCamera.h"
#include "modules/ModuleTexture.h"
#include "modules/ModuleProgram.h"
#include "modules/ModuleHardware.h"
#include "modules/ModuleSceneManager.h"
#include "modules/ModuleDebugDraw.h"
#include "modules/ModuleEvent.h"
#include "modules/ModuleFileSystem.h"
#include "Modules/ModuleResources.h"
#include "Modules/ModuleImporter.h"

#include "Core/preferences/PreferenceManager.h"

using namespace std;

Hachiko::Application::Application()
{
    modules.push_back(hw = new ModuleHardware());
    modules.push_back(file_sys = new ModuleFileSystem());

    modules.push_back(window = new ModuleWindow());
    modules.push_back(input = new ModuleInput());
    modules.push_back(texture = new ModuleTexture());
    modules.push_back(renderer = new ModuleRender());
    modules.push_back(camera = new ModuleCamera());
    modules.push_back(scene_manager = new ModuleSceneManager());
    modules.push_back(program = new ModuleProgram());
    modules.push_back(debug_draw = new ModuleDebugDraw());
    modules.push_back(editor = new ModuleEditor());
    modules.push_back(event = new ModuleEvent());
    modules.push_back(resources = new ModuleResources());
    modules.push_back(importer = new ModuleImporter());

    preferences = new PreferenceManager(SETTINGS_FILE_PATH);
}

Hachiko::Application::~Application()
{
    for (auto it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
    delete preferences;
}

bool Hachiko::Application::Init()
{
    bool ret = true;

    for (auto it = modules.begin(); it != modules.end() && ret; ++it)
    {
        ret = (*it)->Init();
    }

    delta = 0;
    EngineTimer::Start();
    return ret;
}

UpdateStatus Hachiko::Application::Update()
{
    delta = EngineTimer::Update();
    GameTimer::Update();

    auto ret = UpdateStatus::UPDATE_CONTINUE;

    for (auto it = modules.begin(); it != modules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++it)
    {
        ret = (*it)->PreUpdate(static_cast<float>(delta));
    }

    for (auto it = modules.begin(); it != modules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++it)
    {
        ret = (*it)->Update(static_cast<float>(delta));
    }

    for (auto it = modules.begin(); it != modules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++it)
    {
        ret = (*it)->PostUpdate(static_cast<float>(delta));
    }

    return ret;
}

bool Hachiko::Application::CleanUp()
{
    bool ret = true;

    for (auto it = modules.rbegin(); it != modules.rend() && ret; ++it)
    {
        ret = (*it)->CleanUp();
    }

    preferences->SaveConfigurationFile();
    return ret;
}

void Hachiko::Application::RequestBrowser(const char* url)
{
    ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
}
