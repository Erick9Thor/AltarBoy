#pragma once

#include "Window.h"

namespace Hachiko
{
    class WindowAbout final : public Window
    {
    public:
        WindowAbout();

        ~WindowAbout() override;
        void Update() override;
    };
}
