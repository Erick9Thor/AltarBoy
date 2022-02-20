#pragma once

#include "../Resources/ResourceMesh.h"
#include "DrawCommand.h"

#include <vector>

namespace Hachiko
{
    class ComponentMesh;
    
    class GeometryBatch
    {
    public:

        GeometryBatch(ResourceMesh::Layout batch_layout);
        ~GeometryBatch();

        // TODO: Store all geometry data from several meshes
        void AddMesh(const ComponentMesh* mesh);

        void GenerateBatch();
        void GenerateBuffers();
        void GenerateCommands();

        const std::vector<DrawCommand>& GetCommands() const
        {
            return commands;
        }
        
        std::vector<const ComponentMesh*> components; // contains all ComponentMeshes in the batch
        // Commands will be used as templates for the final command list
        std::map<const ResourceMesh*, DrawCommand*> resources; // contains unique ResourceMeshes and their position in the buffer
                
        // We can use resource mesh to contain a concatenation of all original meshes
        ResourceMesh* batch = nullptr;
        unsigned instance_indices_vbo;
        std::vector<DrawCommand> commands;

        bool loaded = false;  
    };

} // namespace Hachiko