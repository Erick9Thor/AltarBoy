#include "core/hepch.h"
#include "ComponentTransform.h"

Hachiko::ComponentTransform::ComponentTransform(GameObject* container) 
    : Component(Type::TRANSFORM, container) 
    , changed(false)
    , matrix(float4x4::identity)
    , matrix_local(float4x4::identity)
    , position(float3::zero)
    , position_local(float3::zero)
    , scale(float3::zero)
    , scale_local(float3::zero)
    , rotation(Quat::identity)
    , rotation_local(Quat::identity)
    , rotation_euler(float3::zero)
    , rotation_euler_local(float3::zero)
    , right(float3::unitX)
    , up(float3::unitY)
    , front(float3::unitZ)
{
}

Hachiko::ComponentTransform::ComponentTransform(GameObject* container, const float3& new_position_local, const Quat& new_rotation_local, const float3& new_scale_local) 
    : ComponentTransform(container)
{
    SetLocalTransform(new_position_local, new_rotation_local, new_scale_local);
}

Hachiko::ComponentTransform::ComponentTransform(GameObject* container, const float4x4& new_transform_local) 
    : ComponentTransform(container)
{
    SetLocalTransform(new_transform_local);
}

void Hachiko::ComponentTransform::OnTransformUpdated()
{
    changed = false;
}

inline Quat Hachiko::ComponentTransform::SimulateLookAt(const float3& direction)
{
    float3 right_temp = float3::unitY.Cross(direction).Normalized();

    float3 up_temp = direction.Cross(right_temp).Normalized();

    Quat orientation = Quat::LookAt(-float3::unitZ, direction, float3::unitY, float3::unitY);

    return orientation;
}

void Hachiko::ComponentTransform::LookAtTarget(const float3& target)
{
    const float3 direction = (GetPosition() - target).Normalized();

    SetRotation(SimulateLookAt(direction));
}

void Hachiko::ComponentTransform::LookAtDirection(const float3& direction) 
{
    SetRotation(SimulateLookAt(direction));
}

void Hachiko::ComponentTransform::SetGlobalTransform(const float4x4& new_matrix)
{
    new_matrix.Decompose(position, rotation, scale);
    rotation_euler = RadToDeg(rotation.ToEulerXYZ());

    UpdateTransformOfHierarchy(MatrixCalculationMode::LOCAL_FROM_GLOBAL);
}

void Hachiko::ComponentTransform::SetPosition(const float3& new_position) 
{
    position = new_position;

    UpdateTransformOfHierarchy(MatrixCalculationMode::LOCAL_FROM_GLOBAL);
}

void Hachiko::ComponentTransform::SetScale(const float3& new_scale) 
{
    scale = new_scale;

    UpdateTransformOfHierarchy(MatrixCalculationMode::LOCAL_FROM_GLOBAL);
}

void Hachiko::ComponentTransform::SetRotationAxis(const float3& new_right, const float3& new_up, const float3& new_front)
{
    matrix.SetCol3(0, new_right);
    matrix.SetCol3(1, new_up);
    matrix.SetCol3(2, new_front);

    matrix.Decompose(position, rotation, scale);

    UpdateTransformOfHierarchy(MatrixCalculationMode::LOCAL_FROM_GLOBAL);
}

void Hachiko::ComponentTransform::SetRotation(const Quat& new_rotation)
{
    rotation = new_rotation.Normalized();
    rotation_euler = RadToDeg(rotation.ToEulerXYZ());

    UpdateTransformOfHierarchy(MatrixCalculationMode::LOCAL_FROM_GLOBAL);
}

void Hachiko::ComponentTransform::SetRotationInEulerAngles(const float3& new_rotation_euler) 
{
    float3 delta = DegToRad(new_rotation_euler - rotation_euler);
    Quat rotation_amount = Quat::FromEulerXYZ(delta.x, delta.y, delta.z).Normalized();

    rotation = rotation_amount * (rotation);
    rotation_euler = new_rotation_euler;

    UpdateTransformOfHierarchy(MatrixCalculationMode::LOCAL_FROM_GLOBAL);
}

void Hachiko::ComponentTransform::SetLocalTransform(const float4x4& new_transform_local)
{
    new_transform_local.Decompose(position_local, rotation_local, scale_local);
    rotation_euler_local = RadToDeg(rotation_local.ToEulerXYZ());

    UpdateTransformOfHierarchy(MatrixCalculationMode::GLOBAL_FROM_LOCAL);
}

void Hachiko::ComponentTransform::SetLocalTransform(const float3& new_position_local, const Quat& new_rotation_local, const float3& new_scale_local)
{
    SetLocalTransform(float4x4::FromTRS(new_position_local, new_rotation_local, new_scale_local));
}

void Hachiko::ComponentTransform::SetLocalPosition(const float3& new_position_local) 
{
    position_local = new_position_local;

    UpdateTransformOfHierarchy(MatrixCalculationMode::GLOBAL_FROM_LOCAL);
}

void Hachiko::ComponentTransform::SetLocalScale(const float3& new_scale_local) 
{
    scale_local = new_scale_local;

    UpdateTransformOfHierarchy(MatrixCalculationMode::GLOBAL_FROM_LOCAL);
}

void Hachiko::ComponentTransform::SetLocalRotation(const Quat& new_rotation_local) 
{
    rotation_local = new_rotation_local;
    rotation_euler_local = RadToDeg(rotation_local.ToEulerXYZ());

    UpdateTransformOfHierarchy(MatrixCalculationMode::GLOBAL_FROM_LOCAL);
}

void Hachiko::ComponentTransform::SetLocalRotationInEulerAngles(const float3& new_rotation_euler_local)
{
    math::float3 delta = DegToRad(new_rotation_euler_local - rotation_euler_local);
    math::Quat rotation_amount = math::Quat::FromEulerXYZ(delta.x, delta.y, delta.z);

    rotation_local = rotation_amount * (rotation_local);
    rotation_euler_local = new_rotation_euler_local;

    UpdateTransformOfHierarchy(MatrixCalculationMode::GLOBAL_FROM_LOCAL);
}
// TODO: Move to serializers
//void Hachiko::ComponentTransform::Save(JsonFormatterValue j_component) const
//{
//    const JsonFormatterValue j_position = j_component["Position"];
//
//    j_position[0] = position_local.x;
//    j_position[1] = position_local.y;
//    j_position[2] = position_local.z;
//
//    const JsonFormatterValue j_rotation = j_component["Rotation"];
//
//    j_rotation[0] = rotation_local.x;
//    j_rotation[1] = rotation_local.y;
//    j_rotation[2] = rotation_local.z;
//    j_rotation[3] = rotation_local.w;
//
//    const JsonFormatterValue j_scale = j_component["Scale"];
//
//    j_scale[0] = scale_local.x;
//    j_scale[1] = scale_local.y;
//    j_scale[2] = scale_local.z;
//}
//
//void Hachiko::ComponentTransform::Load(JsonFormatterValue j_component)
//{
//    const JsonFormatterValue j_position = j_component["Position"];
//    const JsonFormatterValue j_rotation = j_component["Rotation"];
//    const JsonFormatterValue j_scale = j_component["Scale"];
//
//    position_local = float3(j_position[0], j_position[1], j_position[2]);
//    rotation_local = Quat(j_rotation[0], j_rotation[1], j_rotation[2], j_rotation[3]);
//    scale_local = float3(j_scale[0], j_scale[1], j_scale[2]);
//
//    SetLocalTransform(position_local, rotation_local, scale_local);
//}

void Hachiko::ComponentTransform::DrawGui()
{
    static bool locked_scale = true;
    static bool debug_transforms = false;
    
    if (ImGui::CollapsingHeader("Local Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        float3 position_local_editor = position_local;
        float3 scale_local_editor = scale_local;
        float3 rotation_local_editor = rotation_euler_local;
        if (ImGui::DragFloat3("Local Position", position_local_editor.ptr(), 0.1f, -inf, inf))
        {
            SetLocalPosition(position_local_editor);
        }
        if (ImGui::DragFloat3("Local Rotation", rotation_local_editor.ptr(), 0.1f, 0.0f, 360.f))
        {
            SetLocalRotationInEulerAngles(rotation_local_editor);
        }
        if (ImGui::DragFloat3("Local Scale", scale_local_editor.ptr(), 0.1f, 0.0001f, inf, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        {
            SetLocalScale(scale_local_editor);
        }

        ImGui::Checkbox("Debug Transforms", &debug_transforms);
        if (debug_transforms)
        {
            math::float3 position_editor = position;
            math::float3 rotation_editor = rotation_euler;
            math::float3 scale_editor = scale;

            ImGui::Separator();
            if (ImGui::DragFloat3("Position", position_editor.ptr(), 0.1f, -inf, inf))
            {
                SetPosition(position_editor);
            }
            if (ImGui::DragFloat3("Rotation", rotation_editor.ptr(), 0.1f, 0.0f, 360.f))
            {
                SetRotationInEulerAngles(rotation_editor);
            }
            if (ImGui::DragFloat3("Scale", scale_editor.ptr(), 0.1f, 0.0001f, inf, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                SetScale(scale_editor);
            }

            ImGui::Separator();
            ImGui::Text("Local");
            for (int r = 0; r < 4; ++r)
            {
                auto row = matrix_local.Row(r);
                ImGui::Text("%.2f, %.2f, %.2f, %.2f", row[0], row[1], row[2], row[3]);
            }

            ImGui::Separator();
            ImGui::Text("Global");
            for (int r = 0; r < 4; ++r)
            {
                auto row = matrix.Row(r);
                ImGui::Text("%.2f, %.2f, %.2f, %.2f", row[0], row[1], row[2], row[3]);
            }
        }
    }
}

void Hachiko::ComponentTransform::CalculateTransform(MatrixCalculationMode calculation_mode) 
{
    GameObject* owners_parent = game_object->parent;

    switch (calculation_mode)
    {
        case MatrixCalculationMode::LOCAL_FROM_GLOBAL:
        {
            matrix = float4x4::FromTRS(position, rotation, scale);

            matrix_local = owners_parent == nullptr ? matrix : owners_parent->GetTransform()->GetMatrix().Inverted() * matrix;

            matrix_local.Decompose(position_local, rotation_local, scale_local);

            rotation_euler_local = RadToDeg(rotation_local.ToEulerXYZ());
        }
        break;

        case MatrixCalculationMode::GLOBAL_FROM_LOCAL:
        {
            matrix_local = float4x4::FromTRS(position_local, rotation_local, scale_local);

            matrix = owners_parent == nullptr ? matrix_local : owners_parent->GetTransform()->GetMatrix() * matrix_local;

            matrix.Decompose(position, rotation, scale);

            rotation_euler = RadToDeg(rotation.ToEulerXYZ());
        }
        break;
    }

    right = matrix.WorldX();
    up = matrix.WorldY();
    front = matrix.WorldZ();
}

void Hachiko::ComponentTransform::UpdateTransformOfHierarchy(MatrixCalculationMode matrix_calculation_mode)
{
    CalculateTransform(matrix_calculation_mode);

    changed = true;

    const std::vector<GameObject*>& owner_children = game_object->children;

    // As their parent transform is changed only, only updating
    // the global transform matrix of children will suffice.
    for (GameObject* owner_child : owner_children)
    {
        owner_child->GetTransform()->UpdateTransformOfHierarchy(MatrixCalculationMode::GLOBAL_FROM_LOCAL);
    }
}