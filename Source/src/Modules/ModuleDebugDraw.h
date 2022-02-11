#pragma once

#include "Module.h"
#include "Math/float4x4.h"

namespace Hachiko
{
    class DDRenderInterfaceCoreGL;
    class Camera;

    class ModuleDebugDraw : public Module
    {
    public:
        ModuleDebugDraw();
        ~ModuleDebugDraw() override;

        bool Init() override;
        UpdateStatus Update(float delta) override;
        bool CleanUp() override;

        static void Draw(const float4x4& view, const float4x4& proj, unsigned width, unsigned height);

        void SetDebugDraw(bool v)
        {
            debug_draw = v;
        }

        bool debug_draw = true;
        bool draw_quadtree = false;

    private:
        static DDRenderInterfaceCoreGL* implementation;
    };
}
