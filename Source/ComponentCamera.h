#pragma once
#include "Component.h"
#include "Math.h"

class GameObject;

class ComponentCamera :
    public Component
{
    
    public:
        ComponentCamera(GameObject* gameObject);
        ~ComponentCamera();

        void SetNearPlane(float distance);
        void SetFarPlane(float distance);
        void SetFOV(float fov);

        float4x4 GetOpenGLViewMatrix() const;
        float4x4 GetOpenGLProjectionMatrix() const;

    private:
        float hFov = 0.0f;
        Frustum	frustum;
        float3 reference_point = float3::zero;

        Plane planes[6];
};

