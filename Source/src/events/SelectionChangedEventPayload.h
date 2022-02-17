#pragma once

namespace Hachiko
{
    struct SelectionChangedEventPayload
    {
        explicit SelectionChangedEventPayload(GameObject* selected) :
            selected(selected) {}

        [[nodiscard]] GameObject* GetSelected() const
        {
            return selected;
        }

    private:
        GameObject* selected;
    };
}
