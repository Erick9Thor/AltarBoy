#pragma once
#include "Resource.h"

#include "resources/ResourceMesh.h"
#include "components/ComponentMeshRenderer.h"

class dtNavMesh;
class BuildContext;

namespace Hachiko
{
    class ResourceNavMesh : public Resource
    {
    public:
        ResourceNavMesh(UID uid);
        ~ResourceNavMesh() override;

        bool Build(Scene* scene);    // Creates NavMesh for the scene level
        void DebugDraw();                               // Draw debug info
    private:
        void CleanUp();

        //void SetupNavMeshParams();                      // Setup Params prior to nav mesh building
    private:
        dtNavMesh* navmesh = nullptr;
        BuildContext* build_context = nullptr;

        // Build Params
        // Agent
        float agent_height = 2.0f;
        float agent_radius = 0.5f;
        float agent_max_climb = 0.9f;
        float agent_max_slope = 45.0f;

        // Rasterization
        float cell_size = 0.5f;
        float cell_height = 0.20f;

        // Region
        int region_min_size = 8;
        int region_merge_size = 20;

        // Polygonization
        int edge_max_length = 12;
        int edge_max_error = 1.3f;
        int max_vertices_per_poly = 6;

        // Detail
        int detail_sample_distance = 6;
        int detail_sample_max_error = 1;

        // Tiling? 
    };
}