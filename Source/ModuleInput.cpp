#include "Globals.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "SDL.h"

#include "Application.h"
#include "ModuleScene.h"

#include "imgui_impl_sdl.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("[M_INPUT] Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("[M_INPUT] SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

    keyboard = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
        mouse_buttons[i] = false;

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
    SDL_PumpEvents();

    SDL_Event event;

    mouse_wheel = false;
    mouse_motion_x = mouse_motion_y = 0;
    mouse_wheel_x = mouse_wheel_y = 0;

    while (SDL_PollEvent(&event) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(event.window.data1, event.window.data2);
                    // App->camera->WindowResized(event.window.data1, event.window.data2);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_buttons[event.button.button - 1] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_buttons[event.button.button - 1] = false;
                break;
            case SDL_MOUSEMOTION:
                mouse_motion_x = event.motion.xrel;
                mouse_motion_y = event.motion.yrel;
                break;
            case SDL_MOUSEWHEEL:
                mouse_wheel = true;
                mouse_wheel_x = event.wheel.x;
                mouse_wheel_y = event.wheel.y;
                break;
            case SDL_DROPFILE: 
            {
                LOG("[M_INPUT] Dropped file: %s", event.drop.file);

                App->scene->LoadModel(event.drop.file);
                
                SDL_free(event.drop.file);
                break;
            }
        }
    }

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("[M_INPUT] Quitting SDL input event subsystem");
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