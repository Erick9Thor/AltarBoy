#pragma once

#include "Module.h"
#include <SDL.h>

namespace Hachiko
{
    class Application;

    class ModuleWindow : public Module
    {
    public:
        ModuleWindow();
        ~ModuleWindow() override;

        bool Init() override;
        [[nodiscard]] bool CleanUp() override;

        void WindowResized();
        void SetFullScreen(bool fullscreen) const;
        void SetResizable(bool resizable) const;
        void SetSize(int w, int h) const;
        static void SetVsync(bool vsync);

        [[nodiscard]] int GetWidth() const
        {
            return width;
        }

        [[nodiscard]] int GetHeight() const
        {
            return height;
        }

        void OptionsMenu();

        [[nodiscard]] SDL_Surface* GetScreenSurface() const
        {
            return screen_surface;
        }

        [[nodiscard]] SDL_Window* GetWindow() const
        {
            return window;
        }

    private:
        static void GetMonitorResolution(int& width, int& height);

        SDL_Window* window = nullptr;
        SDL_Surface* screen_surface = nullptr;

        bool fullscreen{};
        bool resizable{};
        int width{};
        int height{};
        int max_width{};
        int max_height{};
        int refresh_rate{};
        bool vsync{};
    };
}