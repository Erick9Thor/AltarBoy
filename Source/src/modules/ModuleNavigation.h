#pragma once

#include "Module.h"

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

        void DebugDraw();
        
        float GetYFromPosition(const math::float3& position) const;

    private:
        ResourceNavMesh* navmesh = nullptr;
    };
}