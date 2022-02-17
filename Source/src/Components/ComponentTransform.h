#pragma once
#include "Component.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform : public Component
    {
    public:
        ComponentTransform(GameObject* new_object, float3 position = float3::zero, Quat rotation = Quat::identity, float3 scale = float3::one);
        ComponentTransform(GameObject* new_object, const float4x4& transform = float4x4::identity);
        ~ComponentTransform() override;

        static Type GetType()
        {
            return Type::TRANSFORM;
        }

        void SetPosition(float3 new_position);
        void SetRotationAxis(float3 x, float3 y, float3 z);
        void LookAt(float3 target, float3 world_up = float3::unitY);

        void SetLocalTransform(float4x4 new_transform);
        void SetLocalTransform(float3 position, Quat rotation, float3 scale);

        inline void SetLocalPosition(float3 new_position);
        inline void SetLocalScale(float3 new_scale);
        inline void SetLocalRotation(Quat new_rotation);
        inline void SetLocalRotation(float3 rotation_angles);

        void SetGlobalTransform(float4x4 transform);
        void UpdateGlobalTransformHierarchy();

        void OnTransformUpdated() override
        {
            changed = false;
        }

        [[nodiscard]] bool HasChanged() const
        {
            return changed;
        }

        [[nodiscard]] float4x4 GetTransform() const
        {
            return transform;
        }

        [[nodiscard]] float3 GetPosition() const
        {
            return transform.TranslatePart();
        }

        [[nodiscard]] float3 GetFwd() const
        {
            return transform.WorldZ();
        }

        [[nodiscard]] float3 GetUp() const
        {
            return transform.WorldY();
        }

        [[nodiscard]] float3 GetRight() const
        {
            return transform.WorldX();
        }

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;

    private:
        bool changed = true;

        float4x4 transform;

        float4x4 local_transform;
        float3 local_position;
        float3 local_scale;
        Quat local_rotation;
        float3 local_rotation_euler;
    };
}
