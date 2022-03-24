#pragma once

#include "Resource.h"

namespace Hachiko
{
    class ResourceNode
    {
    public:
        ~ResourceNode() 
        {
            for (auto child : childs)
            {
                delete child;
            }
        }

        std::string node_name;
        float4x4 node_transform;
        UID mesh_id;
        UID material_id;
        std::vector<ResourceNode*> childs {};
    };

    class ResourceModel final : public Resource
    {
    public:
        ResourceModel() : Resource(Resource::Type::MODEL){};
        ResourceModel(UID uid);
        ~ResourceModel() override;

        [[nodiscard]] unsigned GetMeshIndex(UID id) const
        {
            for (auto child : child_nodes)
            {

            }
        }

        std::vector<ResourceNode*> child_nodes;
        std::string model_path;
        std::string model_name;
    };
}
