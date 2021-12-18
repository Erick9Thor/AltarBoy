#pragma once
#include "Module.h"
#include "Globals.h"

#include "SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:

	ModuleInput();
	~ModuleInput() override;

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	bool CleanUp() override;

	const unsigned GetKey(SDL_Scancode key) const { return keyboard[key]; }
	const bool GetKeyMod(SDL_Keymod modifier) const { return (keymods & modifier); }
	const bool GetMouseButton(int button) const { return (mouse & SDL_BUTTON(button)); }
	const void GetMouseDelta(int& x, int& y) const { x = mouse_delta_x; y = mouse_delta_y; }
	int GetScrollDelta() const { return scroll_delta; }

private:
	void UpdateInputMaps();
	const Uint8* keyboard = NULL;
	Uint32 mouse;
	SDL_Keymod keymods;
	int mouse_x, mouse_y;
	int mouse_delta_x, mouse_delta_y;
	int scroll_delta;
};