#pragma once

#include "GeometryBatch.h"
#include "TextureBatch.h"

#include <vector>

namespace Hachiko
{
    class ComponentMesh;
    
    class BatchManager
    {
    public: 
        ~BatchManager();

        void AddMesh(const ComponentMesh* mesh);

        void GenerateDynamicBuffers();
        void BuildBatches();
        void DrawBatches();

        void CleanUp();
        unsigned transform_ssbo;
        unsigned material_ssbo;
        // One GeometryBatch per attribute layout
        std::vector<GeometryBatch*> geometry_batches;
        std::vector<TextureBatch*> texture_batches;
        
    };
} // namespace Hachiko
