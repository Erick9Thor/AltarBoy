#pragma once

#include "Resource.h"
#include "yaml-cpp/node/node.h"


namespace Hachiko
{
    class ResourcePrefab : public Resource
    {
    public:
        ResourcePrefab();
        ResourcePrefab(UID id);
        ~ResourcePrefab() override = default;
        YAML::Node prefab_data;
        std::string name;
    };
} // namespace Hachiko
