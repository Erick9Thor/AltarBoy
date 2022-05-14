#pragma once

#include "Window.h"

namespace Hachiko
{
    class WindowResource final : public Window
    {
    public:
        WindowResource();
        ~WindowResource() override = default;

        void Update() override;
    private:
        void LoadResource(const std::string& path);
        void LoadMaterialIntoSelectedObject(YAML::Node& node);
        std::filesystem::path current_directory = "./assets";
        std::string auxiliary_name;

    };
}
