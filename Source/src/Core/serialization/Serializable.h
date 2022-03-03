#pragma once

#include "yaml-cpp/yaml.h"

namespace Hachiko
{
    class Serializable
    {
    public:
        virtual ~Serializable() = default;
        virtual void Save(YAML::Node) = 0;
        virtual void Load(YAML::Node) = 0;
    };
}
