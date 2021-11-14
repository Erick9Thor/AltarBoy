#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

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

public:
	SDL_Window* window = NULL;
	SDL_Surface* screen_surface = NULL;
};

#endif // __ModuleWindow_H__