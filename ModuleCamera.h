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

    private:
        Frustum frustum;

};

