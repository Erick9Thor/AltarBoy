#pragma once
#include "Component.h"
#include "GameObject.h"

class GameObject;

class ComponentTransform :
    public Component
{
    public:
        ComponentTransform(GameObject* new_GameObject, const float4x4& transform = float4x4::identity);
        ~ComponentTransform();

        void LookAt(float3 target, float3 worldUp = float3::unitY);

    private:
        float4x4 local_transform;
        float4x4 transform;

        float3 local_position;
        float3 local_scale;
        Quat local_rotation;
        float3 local_rotation_euler;
};

