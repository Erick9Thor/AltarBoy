#pragma once

#include "Window.h"
#include "Utils/Timer.h"

namespace Hachiko
{
    class WindowTimers final : public Window
    {
    public:
        WindowTimers();
        ~WindowTimers() override;

        void Update() override;

    private:
        Timer timer;
    };
}
