#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"

#include "Leaks.h"

ModuleWindow::ModuleWindow()
{
}

ModuleWindow::~ModuleWindow()
{
}

bool ModuleWindow::Init()
{
	LOG("[M_Window] Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("[M_Window] SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);

		width = DM.w * 0.75f;
		height = DM.h * 0.75f;

		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if (FULLSCREEN == true)
			flags |= SDL_WINDOW_FULLSCREEN;

		if (RELIZABLE == true)
			flags |= SDL_WINDOW_RESIZABLE;


		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("[M_Window] Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("[M_Window] Destroying SDL window and quitting all SDL systems");

	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
	return true;
}

void ModuleWindow::SetFullscreen(bool set) {
	if (set != fullscreen)
	{
		fullscreen = set;
		if (fullscreen == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
				LOG("[M_Window] Could not switch to fullscreen: %s\n", SDL_GetError());
			fullscreen_desktop = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				LOG("[M_Window] Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

bool ModuleWindow::IsFullscreen() const
{
	return fullscreen;
}

bool ModuleWindow::IsResizable() const
{
	return resizable;
}

void ModuleWindow::WindowResized()
{
	SDL_UpdateWindowSurface(window);
	screen_surface = SDL_GetWindowSurface(window);
	App->renderer->WindowResized(screen_surface->w, screen_surface->h);
	App->camera->SetAspectRatio(screen_surface->w, screen_surface->h);
}

void ModuleWindow::SetResizable(bool set) {
	resizable = set;
}

