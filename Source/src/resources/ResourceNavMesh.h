#pragma once
#include "Resource.h"

#include "resources/ResourceMesh.h"
#include "components/ComponentMeshRenderer.h"


class dtNavMesh;
class dtNavMeshQuery;
class dtCrowd;
class dtTileCache;
class BuildContext;
class LinearAllocator;
class FastLZCompressor;
class MeshProcess;
class DebugDrawGL;


namespace Hachiko
{
    enum SamplePartitionType
    {
        SAMPLE_PARTITION_WATERSHED,
        SAMPLE_PARTITION_MONOTONE,
        SAMPLE_PARTITION_LAYERS,
    };

    // These are just sample areas, we have to define ours
    enum SamplePolyAreas
    {
        SAMPLE_POLYAREA_GROUND,
        SAMPLE_POLYAREA_WATER,
        SAMPLE_POLYAREA_ROAD,
        SAMPLE_POLYAREA_DOOR,
        SAMPLE_POLYAREA_GRASS,
        SAMPLE_POLYAREA_JUMP,
    };

    enum SamplePolyFlags
    {
        SAMPLE_POLYFLAGS_WALK = 0x01, // Ability to walk (ground, grass, road)
        SAMPLE_POLYFLAGS_SWIM = 0x02, // Ability to swim (water).
        SAMPLE_POLYFLAGS_DOOR = 0x04, // Ability to move through doors.
        SAMPLE_POLYFLAGS_JUMP = 0x08, // Ability to jump.
        SAMPLE_POLYFLAGS_DISABLED = 0x10, // Disabled polygon
        SAMPLE_POLYFLAGS_ALL = 0xffff // All abilities.
    };

    class ResourceNavMesh : public Resource
    {
    public:
        friend class ModuleNavigation;
        ResourceNavMesh(UID uid);
        ~ResourceNavMesh() override;

        dtCrowd* GetCrowd() const { return crowd; }
        dtNavMeshQuery* GetQuery() const { return navigation_query; }

        bool Build(Scene* scene);    // Creates NavMesh for the scene level
        void DebugDraw(DebugDrawGL& dd); // Draw debug info

    public:
       // Build Params
        struct NavmeshParams
        {
            // Agent
            float agent_height = 2.0f;
            float agent_radius = 0.5f;
            float agent_max_climb = 0.9f;
            float agent_max_slope = 60.0f;

            // Rasterization
            float cell_size = 0.3f;
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
        };

        NavmeshParams build_params;
        void DrawOptionsGui();
        
    private:
        void CleanUp();

        //void SetupNavMeshParams();                      // Setup Params prior to nav mesh building
    private:
        dtNavMesh* navmesh = nullptr;
        dtNavMeshQuery* navigation_query = nullptr;
        dtTileCache* tile_cache = nullptr;
        dtCrowd* crowd = nullptr;
        BuildContext* build_context = nullptr;

        LinearAllocator* talloc = nullptr;
        FastLZCompressor* tcomp = nullptr;
        MeshProcess* tmproc = nullptr;

        // Partition
        SamplePartitionType partition_type = SAMPLE_PARTITION_WATERSHED;

        // Tiling? 
        int tile_size = 48;

    public:
        static const int EXPECTED_LAYERS_PER_TILE = 4;
        static const int MAX_LAYERS = 32;
        static const int MAX_AGENTS = 128;
        static const int AGENT_MAX_TRAIL = 64;
    };
}