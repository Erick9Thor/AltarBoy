#pragma once
#include "Window.h"

namespace Hachiko
{
    class WindowResource final : public Window
    {
    public:
        WindowResource();
        ~WindowResource() override;
        void Update() override;
    };
}
