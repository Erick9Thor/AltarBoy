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
        // vbo, ebo, vao & layout can be retrieved from batch
        ResourceMesh* batch = nullptr;
        std::vector<DrawCommand> commands;
        
        unsigned index_to_fill = 0;
        unsigned vertex_offset = 0;
        unsigned index_offset = 0;    
    };

} // namespace Hachiko