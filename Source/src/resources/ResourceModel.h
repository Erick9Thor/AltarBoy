#pragma once

#include "Resource.h"

namespace Hachiko
{
    struct ResourceNode
    {
        ~ResourceNode() 
        {
            for (auto child : childs)
            {
                delete child;
            }
        }

        UID mesh_id;
        std::string material_name;
        std::string node_name;
        float4x4 node_transform;
        std::vector<int> meshes_index;
        std::vector<ResourceNode*> childs {};
    };

    class ResourceModel final : public Resource
    {
    public:
        ResourceModel() : Resource(Resource::Type::MODEL){};
        ResourceModel(UID uid);
        ~ResourceModel() override;

        std::vector<ResourceNode*> child_nodes;
        std::string model_path;
        std::string material_path;
        std::string model_name;
    };
}
