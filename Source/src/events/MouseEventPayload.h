#pragma once

#include <MathGeolib.h>

namespace Hachiko
{
    struct MouseEventPayload
    {
        enum Action
        {
            CLICK,
            RELEASE,
        };

        MouseEventPayload(Action action, float2 coords) : action(action), coords(coords) {}

        [[nodiscard]] Action GetAction() const
        {
            return action;
        }

        [[nodiscard]] float2 GetCoords() const
        {
            return coords;
        }

    private:
        float2 coords;
        Action action;
    };
}