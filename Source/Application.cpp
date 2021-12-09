#pragma once
#include "Application.h"
#include "ImGuiComponents/AppLog.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleHardware.h"
#include "ModuleSceneManager.h"
#include "ModuleDebugDraw.h"

using namespace std;

Application::Application(): frames(0), last_frame_ms(-1), last_fps(-1), capped_ms(1000 / 60)
{
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(hw = new ModuleHardware());
	modules.push_back(scene = new ModuleSceneManager());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(debug_draw = new ModuleDebugDraw());
	modules.push_back(editor = new ModuleEditor());
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
	delta_time = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	update_status ret = UPDATE_CONTINUE;

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	FinishUpdate();
	return ret;
}

void Application::FinishUpdate()
{
	++frames;
	++fps_counter;

	if (fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	if (capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);

	editor->AddFPS((float)last_fps, (float)last_frame_ms);
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
