#pragma once
#include "Module.h"
#include "Math.h"

class ModuleCamera :
    public Module
{
    public:
        ModuleCamera();
        ~ModuleCamera();

        void OnDebugDraw(bool selected) const;
        
        //008 - Wraping frustrum of the camera
        void SetFOV(float fov);
        void SetAspectRatio(float aspect_ratio);

        float4x4 getProjectionMatrix();
        float4x4 getViewMatrix();

    private:
        Frustum frustum;
        float4x4 projectionGL;
        float4x4 viewGL;
};

