#pragma once

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:
	ModuleWindow();
	~ModuleWindow() override;

	bool Init() override;
	bool CleanUp() override;

	void WindowResized();
	void SetFullScreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetSize(int w, int h);
	void SetVsync(bool vsync);

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	void OptionsMenu();

	inline SDL_Surface* GetScreenSurface() const { return screen_surface; }
	inline SDL_Window* GetWindow() const { return window; }

private:
	void GetMonitorResolution(int& width, int& height);

	SDL_Window* window = NULL;
	SDL_Surface* screen_surface = NULL;

	bool fullscreen;
	bool resizable;
	int width;
	int height;
	int max_width;
	int max_height;	
	int refresh_rate;
	bool vsync;
};
