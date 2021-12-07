#pragma once
#include "Component.h"
#include "GameObject.h"

#include "Math.h"

class GameObject;

class ComponentTransform :
    public Component
{
    public:
        ComponentTransform(GameObject* new_GameObject, float3 position = float3::zero, Quat rotation = Quat::identity, float3 scale = float3::one);
        ComponentTransform(GameObject* new_GameObject, const float4x4& transform = float4x4::identity);
        ~ComponentTransform();

        void SetPosition(float3 newPosition);

        void SetLocalTransform(float4x4 newTransform);
        void SetGlobalTransform(float4x4 transform);

        void SetRotationAxis(float3 x, float3 y, float3 z);

        inline void SetLocalPosition(float3 newPosition) { SetLocalTransform(newPosition, local_rotation, local_scale); }
        inline void SetLocalTransform(float3 position, Quat rotation, float3 scale) { SetLocalTransform(float4x4::FromTRS(position, rotation, scale)); }

        inline float4x4 GetTransform() const { return transform; }

        inline float3 GetPosition() const { return transform.TranslatePart(); }
        inline float3 GetFwd() const { return transform.WorldZ(); }
        inline float3 GetUp() const { return transform.WorldY(); }
        inline float3 GetRight() const { return transform.WorldX(); }

        void LookAt(float3 target, float3 worldUp = float3::unitY);

        static inline Type GetType() { return Type::Transform; };
        void OnTransformUpdated() override;


    private:
        float4x4 local_transform;
        float4x4 transform;

        float3 local_position;
        float3 local_scale;
        Quat local_rotation;
        float3 local_rotation_euler;
};

