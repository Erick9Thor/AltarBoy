#pragma once
#include "Module.h"
#include "Globals.h"

#include "SDL.h"

typedef unsigned __int8 Uint8;

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	ModuleInput();
	~ModuleInput() override;

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	bool CleanUp() override;

	KeyState GetKey(SDL_Scancode key) const { return keyboard[key]; }
	const bool GetKeyMod(SDL_Keymod modifier) const { return (keymods & modifier); }
	const bool GetMouseButton(int button) const { return (mouse & SDL_BUTTON(button)); }
	int GetScrollDelta() const { return scroll_delta; }
	void GetMouseDelta(int& x, int& y) const
	{
		x = mouse_delta_x;
		y = mouse_delta_y;
	}

	void GetMousePosition(int& x, int& y) const
	{
		x = mouse_x;
		y = mouse_y;
	}

private:
	void UpdateInputMaps();

	KeyState* keyboard = nullptr;
	Uint32 mouse;
	SDL_Keymod keymods;

	int mouse_x, mouse_y;
	int mouse_delta_x, mouse_delta_y;
	int scroll_delta;
};