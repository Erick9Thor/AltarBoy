#pragma once

namespace Hachiko
{
    struct NavmeshLoadEventPayload
    {
        enum State
        {
            LOADED,
            NOT_LOADED,
        };

        NavmeshLoadEventPayload(State state) : state(state) {}

        [[nodiscard]] State GetState() const
        {
            return state;
        }

    private:
        State state;
    };
} // namespace Hachiko