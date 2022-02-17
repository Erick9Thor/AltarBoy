#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Module.h"
#include "Resources/Resource.h"
//#include "core/preferences/editor/ResourcesPreferences.h"

namespace Hachiko
{
    class ModuleResources : public Module
    {
    private:
        std::vector<std::pair<Hachiko::Resource::Type, std::string>> supported_extensions = {
            {Hachiko::Resource::Type::TEXTURE, ".png"},
            {Hachiko::Resource::Type::TEXTURE, ".tif"},
            {Hachiko::Resource::Type::MODEL, ".fbx"},
            {Hachiko::Resource::Type::SCENE, ".be"}
        };
        Hachiko::ResourcesPreferences* preferences = nullptr;
        std::filesystem::path last_resource_path; // TODO: This will track every resource, his type and path loaded

        void HandleAssetsChanged(const std::filesystem::path& asset, Hachiko::Resource::Type asset_type);
        void HandleResource(const std::filesystem::path& path);

    public:
        bool Init() override;
        bool CleanUp() override;

        [[nodiscard]] std::filesystem::path GetLastResourceLoadedPath() const;
        Hachiko::Resource::Type GetType(const std::filesystem::path& file);
    };
}
