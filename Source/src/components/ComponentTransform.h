#pragma once
#include "Component.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform : public Component
    {

    public:
        ComponentTransform(GameObject* container);
        ComponentTransform(GameObject* new_object, const float3& new_position_local, const Quat& new_rotation_local, const float3& new_scale_local);
        ComponentTransform(GameObject* new_object, const float4x4& matrix);
        ~ComponentTransform() override = default;

        void OnTransformUpdated() override;

        static Quat SimulateLookAt(const math::float3& direction);
        void LookAtTarget(const float3& target);
        void LookAtDirection(const float3& direction);

        void SetLocalTransform(const float4x4& new_transform_local);
        void SetLocalTransform(const float3& new_position_local, const Quat& new_rotation_local, const float3& new_scale_local);        
        void SetGlobalTransform(const float4x4& new_transform);
        void SetGlobalTransform(const float3& new_position, const Quat& new_rotation, const float3& new_scale);
        void SetLocalPosition(const float3& new_position_local);
        void SetLocalScale(const float3& new_scale_local);
        void SetLocalRotation(const Quat& new_rotation_local);
        void SetLocalRotationEuler(const float3& new_rotation_euler_local);
        void SetGlobalRotationAxis(float3 x, float3 y, float3 z);

        void SetGlobalPosition(const float3& new_position);
        void SetGlobalScale(const float3& new_scale);
        void SetGlobalRotation(const Quat& new_rotation);
        void SetGlobalRotationEuler(const float3& new_rotation_euler);
        

        static Type GetType() { return Type::TRANSFORM; };
        [[nodiscard]] bool IsDirty() const { return dirty; }
        [[nodiscard]] bool HasChanged() const { return changed; }        

        [[nodiscard]] const float3& GetLocalPosition();
        [[nodiscard]] const float3& GetLocalScale();
        [[nodiscard]] const Quat& GetLocalRotation();
        [[nodiscard]] const float3& GetLocalRotationEuler();
        [[nodiscard]] const float3& GetFront();
        [[nodiscard]] const float3& GetUp();
        [[nodiscard]] const float3& GetRight();
        [[nodiscard]] const float4x4& GetMatrix();
        [[nodiscard]] const float4x4& GetLocalMatrix();
        
        [[nodiscard]] const float3& GetPosition();
        [[nodiscard]] const float3& GetScale();
        [[nodiscard]] const Quat& GetRotation();
        [[nodiscard]] const float3& GetRotationEuler();




        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;        

    private:
        void Invalidate();
        void UpdateTransform();  


    private:
        bool dirty;
        bool changed;

        float4x4 matrix;
        float4x4 matrix_local;

        float3 local_position;
        float3 local_scale;
        Quat local_rotation;
        float3 local_rotation_euler;

        float3 position;
        float3 scale;
        Quat rotation;
        float3 rotation_euler;

        float3 right;
        float3 up;
        float3 front;
    };
} // namespace Hachiko
