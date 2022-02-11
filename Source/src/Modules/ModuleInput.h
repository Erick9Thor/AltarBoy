#pragma once
#include "Module.h"
#include "Globals.h"

#include "SDL.h"

namespace Hachiko
{
    using Uint8 = unsigned __int8;

    enum class KeyState
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
        UpdateStatus PreUpdate(float delta) override;
        bool CleanUp() override;

        [[nodiscard]] KeyState GetKey(SDL_Scancode key) const
        {
            return keyboard[key];
        }

        [[nodiscard]] bool GetKeyMod(SDL_Keymod modifier) const
        {
            return (keymods & modifier);
        }

        [[nodiscard]] bool GetMouseButton(int button) const
        {
            return (mouse & SDL_BUTTON(button));
        }

        [[nodiscard]] int GetScrollDelta() const
        {
            return scroll_delta;
        }

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
        Uint32 mouse{};
        SDL_Keymod keymods;

        int mouse_x{}, mouse_y{};
        int mouse_delta_x{}, mouse_delta_y{};
        int scroll_delta{};
    };
}
