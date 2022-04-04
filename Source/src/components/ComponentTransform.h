#pragma once

#include "Component.h"

#if defined(HACHIKO_API)
// Do Nothing
#elif defined(_MSC_VER)
#define HACHIKO_API __declspec(dllexport)
#endif

namespace Hachiko
{
    class GameObject;

    class HACHIKO_API ComponentTransform : public Component
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

        static Type GetType();
        [[nodiscard]] bool HasChanged() const;
        [[nodiscard]] const float3& GetPosition() const;
        [[nodiscard]] const float3& GetLocalPosition() const;
        [[nodiscard]] const float3& GetScale() const;
        [[nodiscard]] const float3& GetLocalScale() const;
        [[nodiscard]] const Quat& GetRotation() const;
        [[nodiscard]] const Quat& GetLocalRotation() const;
        [[nodiscard]] const float3& GetRotationInEulerAngles() const;
        [[nodiscard]] const float3& GetLocalRotationInEulerAngles() const;
        [[nodiscard]] const float3& GetFront() const;
        [[nodiscard]] const float3& GetUp() const;
        [[nodiscard]] const float3& GetRight() const;
        [[nodiscard]] const float4x4& GetMatrix() const;
        [[nodiscard]] const float4x4& GetLocalMatrix() const;
       
        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;

    private:
        void CalculateTransform(MatrixCalculationMode calculation_mode);
        void UpdateTransformOfHierarchy(MatrixCalculationMode matrix_calculation_mode);

    private:
        bool        changed = true;
        float4x4    matrix;
        float4x4    matrix_local;
        float3      position;
        float3      position_local;
        float3      scale;
        float3      scale_local;
        Quat        rotation;
        Quat        rotation_local;
        float3      rotation_euler;
        float3      rotation_euler_local;
        float3      right;
        float3      up;
        float3      front;
    };
}

inline Hachiko::Component::Type Hachiko::ComponentTransform::GetType()
{
    return Type::TRANSFORM;
}

inline bool Hachiko::ComponentTransform::HasChanged() const
{
    return changed;
}

inline const float3& Hachiko::ComponentTransform::GetPosition() const
{
    return position;
}

inline const float3& Hachiko::ComponentTransform::GetLocalPosition() const
{
    return position_local;
}

inline const float3& Hachiko::ComponentTransform::GetScale() const
{
    return scale;
}

inline const float3& Hachiko::ComponentTransform::GetLocalScale() const
{
    return scale_local;
}

inline const Quat& Hachiko::ComponentTransform::GetRotation() const
{
    return rotation;
}

inline const Quat& Hachiko::ComponentTransform::GetLocalRotation() const
{
    return rotation_local;
}

inline const float3& Hachiko::ComponentTransform::GetRotationInEulerAngles() const
{
    return rotation_euler;
}

inline const float3& Hachiko::ComponentTransform::GetLocalRotationInEulerAngles() const
{
    return rotation_euler_local;
}

inline const float3& Hachiko::ComponentTransform::GetFront() const
{
    return front;
}

inline const float3& Hachiko::ComponentTransform::GetUp() const
{
    return up;
}

inline const float3& Hachiko::ComponentTransform::GetRight() const
{
    return right;
}

inline const float4x4& Hachiko::ComponentTransform::GetMatrix() const
{
    return matrix;
}

inline const float4x4& Hachiko::ComponentTransform::GetLocalMatrix() const
{
    return matrix_local;
}