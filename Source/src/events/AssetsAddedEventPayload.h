#pragma once

#include <filesystem>
#include "resources/Resource.h"

namespace Hachiko
{
    struct AssetsAddedEventPayload
    {
        AssetsAddedEventPayload(std::filesystem::path path, const Resource::Type type) :
            path(std::move(path)),
            type(type) {}

        [[nodiscard]] std::filesystem::path GetPath() const
        {
            return path;
        }

        [[nodiscard]] Resource::Type GetType() const
        {
            return type;
        }

    private:
        std::filesystem::path path;
        Resource::Type type;
    };
}
