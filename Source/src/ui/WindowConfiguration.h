#pragma once
#include "Window.h"

namespace Hachiko
{
    class WindowConfiguration final : public Window
    {
    public:
        WindowConfiguration();

        ~WindowConfiguration() override;
        void Update() override;
    };
}
