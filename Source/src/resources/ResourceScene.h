#pragma once

#include "Resource.h"
#include "yaml-cpp/node/node.h"

namespace Hachiko
{
    class ResourceScene : public Resource
    {
    public:
        ResourceScene();
        ResourceScene(UID id);
        ~ResourceScene() override = default;
        YAML::Node scene_data;
        std::string name;
    };
} // namespace Hachiko
