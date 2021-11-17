#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

typedef unsigned __int8 Uint8;

#define NUM_MOUSE_BUTTONS 5

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	const Uint8 GetKey(SDL_Scancode key) const;
	const bool GetMouseButton(unsigned int key) const;
	const void GetMouseMotion(int& x, int& y) const;

private:
	const Uint8* keyboard = NULL;
	bool mouse_buttons[NUM_MOUSE_BUTTONS];
	int mouse_motion_x;
	int mouse_motion_y;
};

#endif // MODULE_INPUT_H