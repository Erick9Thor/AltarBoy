#include "core/hepch.h"

Hachiko::Window::Window(const char* name, const bool active) :
    name(name),
    active(active) {}

Hachiko::Window::~Window() = default;
