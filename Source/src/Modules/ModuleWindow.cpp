#include "../Globals.h"
#include "../Application.h"
#include "../Utils/Logger.h"

#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"

#include "wtypes.h"
#include <imgui.h>

ModuleWindow::ModuleWindow() {}

ModuleWindow::~ModuleWindow() {}

bool ModuleWindow::Init()
{
	GetMonitorResolution(max_width, max_height);
	width = (int) (max_width * WINDOWED_RATIO);
	height = (int) (max_height * WINDOWED_RATIO);

	fullscreen = FULLSCREEN;
	resizable = RESIZABLE;
	vsync = true;

	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

		if (fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN;
		if (resizable)
			flags |= SDL_WINDOW_RESIZABLE;

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			// Get window surface, it must be updated on resize
			screen_surface = SDL_GetWindowSurface(window);
		}
		SDL_DisplayMode mode;
		SDL_GetDisplayMode(0, 0, &mode);
		refresh_rate = mode.refresh_rate;
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::WindowResized()
{
	// Update window surface so it is correct
	SDL_UpdateWindowSurface(window);
	screen_surface = SDL_GetWindowSurface(window);
}

void ModuleWindow::SetFullScreen(bool fullscreen)
{
	// Method returns negative error code on failure
	if (fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetResizable(bool resizable)
{
	SDL_SetWindowResizable(window, (SDL_bool) resizable);
}

void ModuleWindow::SetSize(int w, int h)
{
	SDL_SetWindowSize(window, w, h);
}

void ModuleWindow::SetVsync(bool vsync)
{
	SDL_GL_SetSwapInterval((int) vsync);
}

void ModuleWindow::OptionsMenu()
{
	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->window->SetFullScreen(fullscreen);

	ImGui::SameLine();
	if (ImGui::Checkbox("Vsync", &vsync))
		SetVsync(vsync);

	if (!fullscreen)
	{
		if (ImGui::Checkbox("Resizable", &resizable))
			SetResizable(resizable);
	}
	ImGui::Text("Monitor Refresh Rate: %d", refresh_rate);
}

void ModuleWindow::GetMonitorResolution(int& width, int& height)
{
	RECT monitor;
	const HWND desktop_handle = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(desktop_handle, &monitor);
	width = monitor.right;
	height = monitor.bottom;
}
