#pragma once

#include "Resource.h"
#include "Globals.h"

namespace Hachiko
{
    class ResourceMesh;

    class ResourceModel final : public Resource
    {
    public:
        ResourceModel() : Resource(Resource::Type::MODEL){};
        ResourceModel(UID uid);
        ~ResourceModel() override = default;

    private:
        std::vector<ResourceMesh*> meshes;
    };
}


