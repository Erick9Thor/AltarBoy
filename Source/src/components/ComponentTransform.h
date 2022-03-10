#pragma once
#include "Component.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform : public Component
    {
        enum class MatrixCalculationMode
        {
            USE_GLOBAL_TO_CALC_LOCAL,
            USE_LOCAL_TO_CALC_GLOBAL
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
        inline [[nodiscard]] bool IsDirty() const { return dirty; }; 
        inline [[nodiscard]] const float3& GetLocalPosition() const { return position_local; };
        inline [[nodiscard]] const float3& GetLocalScale() const { return scale_local; };
        inline [[nodiscard]] const Quat& GetLocalRotation() const { return rotation_local; };
        inline [[nodiscard]] const float3& GetLocalRotationInEulerAngles() const { return rotation_euler_local; };
        inline [[nodiscard]] const float4x4& GetLocalMatrix() const { return matrix_local; };

        [[nodiscard]] const float3& GetPosition() const { return position; };
        [[nodiscard]] const float3& GetScale() const { return scale; };
        [[nodiscard]] const Quat& GetRotation() const { return rotation; };
        [[nodiscard]] const float3& GetRotationInEulerAngles() const { return rotation_euler; };
        [[nodiscard]] const float3& GetFront() const { return front; };
        [[nodiscard]] const float3& GetUp() const { return up; };
        [[nodiscard]] const float3& GetRight() const { return right; };
        [[nodiscard]] const float4x4& GetMatrix() const { return matrix; };


        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;

    private:
        void CalculateTransform(MatrixCalculationMode calculation_mode);
        void UpdateTransformOfChildren(MatrixCalculationMode matrix_calculation_mode);

    private:
        bool dirty = true; // Symbolizes whether the GLOBAL transform needs to be re calculated or not .
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
