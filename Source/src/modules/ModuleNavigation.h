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

        // TODO: Implement
        void ChangeNavMesh();
    };
}