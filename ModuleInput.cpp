#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"

#include "imgui_impl_sdl.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

    keyboard = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
        mouse_buttons[i] = false;

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_PumpEvents();

    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_buttons[sdlEvent.button.button - 1] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_buttons[sdlEvent.button.button - 1] = false;
                break;
            case SDL_MOUSEMOTION:
                mouse_motion_x = sdlEvent.motion.xrel;
                mouse_motion_y = sdlEvent.motion.yrel;
                break;
        }
    }

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

const Uint8 ModuleInput::GetKey(SDL_Scancode key) const
{
    return keyboard[key];
}

const bool ModuleInput::GetMouseButton(unsigned int key) const
{
    return mouse_buttons[key - 1];
}

const void ModuleInput::GetMouseMotion(int& x, int& y) const
{
    x = mouse_motion_x;
    y = mouse_motion_y;
}