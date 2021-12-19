#pragma once
#include "Component.h"
#include "Math.h"

class GameObject;

class ComponentCamera :
    public Component
{
    
    public:
        ComponentCamera(GameObject* conatiner);
        ~ComponentCamera() override;

        void SetNearPlane(float distance);
        void SetFarPlane(float distance);
        void SetFOV(float fov);

        float4x4 GetViewMatrix(const bool transpose = false) const;
        float4x4 GetProjectionMatrix(const bool transpose = false) const;

        void OnTransformUpdated() override;

        void SetResolution(float width, float height);
        
        static inline Type GetType() { return Type::Camera; };

        inline unsigned int GetRenderTarget() const { return renderTexture; }

        Plane planes[6];
        float3 reference_point = float3::zero;

    private:
        float horizontal_fov;
        Frustum	frustum;
        unsigned int renderTexture = 0;
};

