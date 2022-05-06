#include "core/hepch.h"

#include "ModuleNavigation.h"
#include "core/Scene.h"
#include "ModuleSceneManager.h"
#include "resources/ResourceNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourTileCache.h"
#include "DetourCrowd.h"

Hachiko::ModuleNavigation::ModuleNavigation() {}

Hachiko::ModuleNavigation::~ModuleNavigation() {}

bool Hachiko::ModuleNavigation::Init()
{
    return true;
}

bool Hachiko::ModuleNavigation::CleanUp()
{
    // TODO: Manage as a resources aand not released here
    RELEASE(navmesh);
    return true;
}

bool Hachiko::ModuleNavigation::BuildNavmesh(Scene* scene)
{
    HE_LOG("Building Navmesh");
    RELEASE(navmesh);

    navmesh = new ResourceNavMesh(0);
    bool success = navmesh->Build(scene);
    if (!success)
    {
        HE_LOG("Failed to build navmesh uwu");
    }

    return success;
}

UpdateStatus Hachiko::ModuleNavigation::Update(const float delta)
{
    if (!navmesh )
    {
        return UpdateStatus::UPDATE_CONTINUE;
    }
    
    dtTileCache* tile_cache = navmesh->tile_cache;
    tile_cache->update(delta, navmesh->navmesh);
    // Note: We can add crowd debug info instead of nullptr
    UpdateObstacleStats(tile_cache);

    navmesh->crowd->update(delta, nullptr);

    return UpdateStatus::UPDATE_CONTINUE;
}

dtTileCache* Hachiko::ModuleNavigation::GetTileCache() const
{
    return navmesh ? navmesh->tile_cache : nullptr;
}

void Hachiko::ModuleNavigation::DebugDraw()
{
    if (navmesh)
    {
        navmesh->DebugDraw();
    }
}

float Hachiko::ModuleNavigation::GetYFromPosition(const math::float3& position) const
{
    float height = FLT_MAX;
    float3 extents = float3(2, 4, 2);

    dtQueryFilter filter;
    dtPolyRef reference;

    

    navmesh->navigation_query->findNearestPoly(position.ptr(), extents.ptr(), &filter, &reference, 0);

    if (reference != NULL)
    {
        navmesh->navigation_query->getPolyHeight(reference, position.ptr(), &height);
    }

    return height;
}

void Hachiko::ModuleNavigation::UpdateObstacleStats(dtTileCache* tile_cache)
{
    total_obstacle_slots = tile_cache->getObstacleCount();
    n_processing = 0;
    n_processed = 0;
    n_removing = 0;

    for (int i = 0; i < total_obstacle_slots; ++i)
    {
        const dtTileCacheObstacle* ob = tile_cache->getObstacle(i);
        switch (ob->state)
        {
        case DT_OBSTACLE_EMPTY:
            break;
        case DT_OBSTACLE_PROCESSED:
            ++n_processed;
            break;
        case DT_OBSTACLE_PROCESSING:
            ++n_processing;
            break;
        case DT_OBSTACLE_REMOVING:
            ++n_removing;
            break;
        }
    }
}
