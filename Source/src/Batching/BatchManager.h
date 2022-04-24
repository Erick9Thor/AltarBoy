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
        BatchManager();
        ~BatchManager();

        void CollectMeshes(const GameObject* game_object);

        void CollectMesh(const GameObject* game_object);

        void AddDrawComponent(const ComponentMesh* mesh);

        void GenerateDynamicBuffers();
        void BuildBatches();
        void DrawBatches();
        void ClearBatchesDrawList();

        void CleanUp();

        unsigned material_ssbo;
        // One GeometryBatch per attribute layout
        std::vector<GeometryBatch*> geometry_batches;
    };
} // namespace Hachiko
