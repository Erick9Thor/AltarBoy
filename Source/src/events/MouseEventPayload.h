#pragma once

namespace Hachiko
{
    struct MouseEventPayload
    {
        enum class Action
        {
            Click,
            Release,
        };

        explicit MouseEventPayload(Action action) : action(action) {}

        [[nodiscard]] Action GetAction() const
        {
            return action;
        }

    private:
        Action action;
    };
}
