#include "ModuleInput.h"

#include "../Globals.h"
#include "../Utils/Logger.h"

#include "../Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleSceneManager.h"

#include "imgui_impl_sdl.h"

#include <string>

ModuleInput::ModuleInput()
{}

ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate(const float delta)
{
	SDL_Event event;
	mouse_delta_x = 0;
	mouse_delta_y = 0;
	scroll_delta = 0;
	while (SDL_PollEvent(&event) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			return UPDATE_STOP;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				App->window->WindowResized();
			break;
		case SDL_MOUSEMOTION:
			mouse_delta_x = event.motion.xrel;
			mouse_delta_y = event.motion.yrel;
			break;
		case SDL_MOUSEWHEEL:
			scroll_delta = event.wheel.y;
			break;
		case SDL_DROPFILE:
			LOG("[M_INPUT] Dropped file: %s", event.drop.file);
			App->scene_manager->LoadModel(event.drop.file);
			SDL_free(event.drop.file);
			break;
		}
	}
	UpdateInputMaps();

	return UPDATE_CONTINUE;
}

void ModuleInput::UpdateInputMaps()
{
	keyboard = SDL_GetKeyboardState(NULL);
	keymods = SDL_GetModState();
	mouse = SDL_GetMouseState(&mouse_x, &mouse_y);
}

bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
