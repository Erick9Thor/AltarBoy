#pragma once
#include "Component.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform : public Component
    {
        enum class MatrixCalculationMode
        {
            LOCAL_FROM_GLOBAL,
            GLOBAL_FROM_LOCAL
        };

    public:
        ComponentTransform(GameObject* container);
        ComponentTransform(GameObject* new_object, const float3& new_position_local, const Quat& new_rotation_local, const float3& new_scale_local);
        ComponentTransform(GameObject* new_object, const float4x4& matrix);
        ~ComponentTransform() override = default;

        void OnTransformUpdated() override;

        static Quat SimulateLookAt(const math::float3& direction);
        void LookAtTarget(const float3& target);
        void LookAtDirection(const float3& direction);

        void SetGlobalTransform(const float4x4& new_matrix);
        void SetPosition(const float3& new_position);
        void SetScale(const float3& new_scale);
        void SetRotationAxis(const float3& new_right, const float3& new_up, const float3& new_front);
        void SetRotation(const Quat& new_rotation);
        void SetRotationInEulerAngles(const float3& new_rotation_euler);
        void SetLocalTransform(const float4x4& new_transform_local);
        void SetLocalTransform(const float3& new_position_local, const Quat& new_rotation_local, const float3& new_scale_local);
        void SetLocalPosition(const float3& new_position_local);
        void SetLocalScale(const float3& new_scale_local);
        void SetLocalRotation(const Quat& new_rotation_local);
        void SetLocalRotationInEulerAngles(const float3& new_rotation_euler_local);

        static Type GetType() { return Type::TRANSFORM; };
        [[nodiscard]] bool IsDirty() const { return dirty; }
        [[nodiscard]] bool HasChanged() const { return changed; }
        [[nodiscard]] const float3& GetLocalPosition() const { return position_local; }
        [[nodiscard]] const float3& GetLocalScale() const { return scale_local; }
        [[nodiscard]] const Quat& GetLocalRotation() const { return rotation_local; }
        [[nodiscard]] const float3& GetLocalRotationInEulerAngles() const { return rotation_euler_local; }
        [[nodiscard]] const float4x4& GetLocalMatrix() const { return matrix_local; }

        [[nodiscard]] const float3& GetPosition();
        [[nodiscard]] const float3& GetScale();
        [[nodiscard]] const Quat& GetRotation();
        [[nodiscard]] const float3& GetRotationInEulerAngles();
        [[nodiscard]] const float3& GetFront();
        [[nodiscard]] const float3& GetUp();
        [[nodiscard]] const float3& GetRight();
        [[nodiscard]] const float4x4& GetMatrix();


        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;

    private:
        void CalculateTransform(MatrixCalculationMode calculation_mode);
        void UpdateTransformAndChildren(MatrixCalculationMode matrix_calculation_mode);
        void Invalidate();

    private:
        bool dirty;
        bool changed;

        float4x4 matrix;
        float4x4 matrix_local;

        float3 position;
        float3 position_local;
        float3 scale;
        float3 scale_local;
        Quat rotation;
        Quat rotation_local;
        float3 rotation_euler;
        float3 rotation_euler_local;

        float3 right;
        float3 up;
        float3 front;
    };
} // namespace Hachiko
