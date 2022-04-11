#pragma once
#include "modules/ModuleInput.h"

namespace Hachiko
{
    struct InputEventPayload
    {

        InputEventPayload(SDL_Scancode key, KeyState state) :
            key(key), state(state) {}

        [[nodiscard]] KeyState GetState() const
        {
            return state;
        }

        [[nodiscard]] SDL_Scancode GetKey(SDL_Scancode key) const
        {
            return key;
        }

    private:        
        SDL_Scancode key;
        KeyState state;
    };
} // namespace Hachiko