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
    
    navmesh->tile_cache->update(delta, navmesh->navmesh);
    // Note: We can add crowd debug info instead of nullptr
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
