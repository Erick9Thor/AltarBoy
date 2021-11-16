#pragma once
#include "Module.h"
#include "Math.h"

class ModuleCamera :
    public Module
{
    public:
        ModuleCamera();
        ~ModuleCamera();
        
        //008 - Wraping frustrum of the camera
        void SetFOV(float fov);
        void SetAspectRatio(float aspect_ratio);

        void SetPlaneDistances(float nearDistance, float farDistance);

        const vec& Pos() const { return frustum.Pos(); }


        float4x4 getProjectionMatrix();
        float4x4 getViewMatrix();

    private:
        Frustum frustum;
        float4x4 projectionGL;
        float4x4 viewGL;
};

