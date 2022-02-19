#pragma once

#include <vector>
#include "../Resources/ResourceMesh.h"

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
        
        std::vector<const ComponentMesh*> components; // contains all ComponentMeshes in the batch
        std::map<const ResourceMesh*, unsigned> resources_index; // contains unique ResourceMeshes and their position in the buffer

        
        // We can use resource mesh to contain a concatenation of all original meshes
        // vbo, ebo, vao & layout can be retrieved from batch
        ResourceMesh* batch = nullptr;
        
        unsigned index_to_fill = 0;
        unsigned vertex_offset = 0;
        unsigned index_offset = 0;

    
    };

} // namespace Hachiko