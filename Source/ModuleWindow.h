#pragma once

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
	public:

		ModuleWindow();
		virtual ~ModuleWindow();

		bool Init();
		bool CleanUp();

		void SetFullscreen(bool set);
		void SetResizable(bool set);

		bool IsFullscreen() const;
		bool IsResizable() const;

		inline SDL_Surface* getScreenSurface() { return screen_surface; }
		inline SDL_Window* getWindow() { return window; }

	private:
		SDL_Window* window = NULL;
		SDL_Surface* screen_surface = NULL;

		bool fullscreen = false;
		bool resizable = false;

		bool fullscreen_desktop = false;
};

