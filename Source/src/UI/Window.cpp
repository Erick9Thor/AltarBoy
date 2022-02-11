#include "Window.h"

Hachiko::Window::Window(const char* name, bool active) :
    name(name),
    active(active) {}

Hachiko::Window::~Window() = default;
