#pragma once

#include "Module.h"

class dtTileCache;

namespace Hachiko
{
    class ResourceNavmesh;
    
    class ModuleNavigation : public Module
    {
    public:
        ModuleNavigation();
        ~ModuleNavigation() override;

        bool Init() override;
        bool CleanUp() override;
        bool BuildNavmesh(Scene* scene);
        UpdateStatus Update(const float delta) override; // Update crowd
        ResourceNavMesh* GetNavMesh() const { return navmesh; };
        dtTileCache* GetTileCache() const;

        void DebugDraw();
        
        float GetYFromPosition(const math::float3& position) const;

    private:
        void UpdateObstacleStats(dtTileCache* tile_cache);
        ResourceNavMesh* navmesh = nullptr;
        int total_obstacle_slots = 0;
        int n_processing = 0;
        int n_processed = 0;
        int n_removing = 0;
    };
}