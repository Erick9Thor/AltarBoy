#pragma once
#include "Window.h"

namespace Hachiko
{
    class GameObject;

    class WindowInspector final : public Window
    {
    public:
        WindowInspector();
        ~WindowInspector() override;
        void Update() override;

    private:
        void DrawGameObject(GameObject* go) const;
    };
}
