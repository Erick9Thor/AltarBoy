#pragma once

#include "Resource.h"
#include <memory>

#include "GLFont.h"

namespace Hachiko
{
    class ResourcePrefab : public Resource
    {
    public:
        ResourcePrefab();
        ResourcePrefab(UID id);
        ~ResourcePrefab() override = default;
    };
} // namespace Hachiko
