#pragma once
#include "Application.h"
#include "ImGuiComponents/AppLog.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "Exercises/ModuleRenderExercise.h"
#include "Exercises/ModuleLoadModels.h"
#include "ModuleDebugDraw.h"
#include "Timer.h";

using namespace std;

Application::Application()
{
	
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(renderer = new ModuleRender());
	// modules.push_back(rendererExercise = new ModuleRenderExercise());
	modules.push_back(moduleLoadModels = new ModuleLoadModels());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(debug_draw = new ModuleDebugDraw());
	modules.push_back(gui = new ModuleGui());

}

Application::~Application()
{
	for(vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it)
}

bool Application::Init()
{
	bool ret = true;

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	float currentTime = SDL_GetTicks();
	deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	Timer fperformance;
	fperformance.StartMicroseconds();


	update_status ret = UPDATE_CONTINUE;

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	LOG("Time expend to ini modules: %f", fperformance.ReadMicroseconds());


	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::DebugDraw()
{
	for (vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
			(*it)->DrawDebug();
}

void Application::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

int Application::GetFramerateLimit() const
{
	if (capped_ms > 0)
		return (int)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}