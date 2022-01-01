#pragma once

#include "Application.h"

#include "Modules/ModuleWindow.h"
#include "Modules/ModuleRender.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleEditor.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleTexture.h"
#include "Modules/ModuleProgram.h"
#include "Modules/ModuleHardware.h"
#include "Modules/ModuleSceneManager.h"
#include "Modules/ModuleDebugDraw.h"

using namespace std;

Application::Application()
{
	modules.push_back(hw = new ModuleHardware());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(scene_manager = new ModuleSceneManager());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(debug_draw = new ModuleDebugDraw());
	modules.push_back(editor = new ModuleEditor());
}

Application::~Application()
{
	for (vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		delete *it;
}

bool Application::Init()
{
	bool ret = true;

	for (vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	delta = 0;
	timer = PerformanceTimer();
	timer.Start();
	prev_tick_time = timer.Read();
	return ret;
}

update_status Application::Update()
{
	double tick_time = timer.Read();
	delta = (tick_time - prev_tick_time) / 1000.0;
	prev_tick_time = tick_time;
	update_status ret = UPDATE_CONTINUE;

	for (vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate((float) delta);

	for (vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update((float) delta);

	for (vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate((float) delta);

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
