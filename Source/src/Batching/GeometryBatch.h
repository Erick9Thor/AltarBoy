#pragma once

#include "../Resources/ResourceMesh.h"
#include "DrawCommand.h"

#include <vector>

namespace Hachiko
{
    class ComponentMesh;
    class TextureBatch;

    struct Material;
    
    class GeometryBatch
    {
    public:

        GeometryBatch(ResourceMesh::Layout batch_layout);
        ~GeometryBatch();

        void AddMesh(const ComponentMesh* mesh);

        void AddDrawComponent(const ComponentMesh* mesh);

        void BuildBatch();
        void BatchMeshes();
        void BatchTransforms();

        void UpdateWithTextureBatch();

        void ClearDrawList();

        void GenerateBuffers();
        void UpdateBuffers();
        void GenerateCommands();        

        void ImGuiWindow();

        const std::vector<DrawCommand>& GetCommands() const
        {
            return commands;
        }
        
        std::vector<const ComponentMesh*> components; // contains all ComponentMeshes in the batch
        // Commands will be used as templates for the final command list
        std::unordered_map<const ResourceMesh*, DrawCommand*> resources; // contains unique ResourceMeshes and their position in the buffer
                
        // We can use resource mesh to contain a concatenation of all original meshes
        ResourceMesh* batch = nullptr;
        unsigned instance_indices_vbo;
        std::vector<float4x4> transforms;
        std::vector<DrawCommand> commands;

        TextureBatch* texture_batch = nullptr;
        unsigned indirect_buffer_id;
    };

} // namespace Hachiko