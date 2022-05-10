#pragma once

#include "Module.h"
#include "DetourCrowd.h"
#include "resources/ResourceNavMesh.h"
#include "SampleInterfaces.h"

class dtTileCache;

namespace Hachiko
{
    
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
        dtCrowd* GetCrowd() const;

        void DebugDraw();
        void DrawOptionsGui();
        
        void CorrectPosition(math::float3& position, const math::float3& extents) const;
        float GetYFromPosition(const math::float3& position) const;

    private:
        void RenderAgents(duDebugDraw& dd);
        void UpdateObstacleStats(dtTileCache* tile_cache);
        ResourceNavMesh* navmesh = nullptr;
        int total_obstacle_slots = 0;
        int n_processing = 0;
        int n_processed = 0;
        int n_removing = 0;
        struct AgentTrail
        {
            float trail[ResourceNavMesh::AGENT_MAX_TRAIL * 3];
            int htrail;
        };
        AgentTrail m_trails[ResourceNavMesh::MAX_AGENTS];
        dtCrowdAgentDebugInfo agent_debug;
        dtObstacleAvoidanceDebugData* avoid_debug = nullptr;
    };
}