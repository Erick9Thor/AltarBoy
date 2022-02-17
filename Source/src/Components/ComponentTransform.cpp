#include "core/hepch.h"
#include "ComponentTransform.h"

Hachiko::ComponentTransform::ComponentTransform(GameObject* new_object, float3 position, Quat rotation, float3 scale) :
    Component(Type::TRANSFORM, new_object),
    local_position(position),
    local_scale(scale),
    local_rotation(rotation)
{
    SetLocalTransform(position, rotation, scale);
}

Hachiko::ComponentTransform::ComponentTransform(GameObject* new_object, const float4x4& new_transform) :
    Component(Type::TRANSFORM, new_object)
{
    SetLocalTransform(new_transform);
}

Hachiko::ComponentTransform::~ComponentTransform() = default;

void Hachiko::ComponentTransform::LookAt(float3 target, float3 world_up)
{
    const float3 direction = target - GetPosition();

    const float3 fwd = direction.Normalized();
    const float3 right = world_up.Cross(fwd).Normalized();
    const float3 up = fwd.Cross(right).Normalized();

    SetRotationAxis(right, up, fwd);
}

void Hachiko::ComponentTransform::SetRotationAxis(float3 x, float3 y, float3 z)
{
    transform.SetCol3(0, x);
    transform.SetCol3(1, y);
    transform.SetCol3(2, z);

    SetGlobalTransform(transform);
}

inline void Hachiko::ComponentTransform::SetLocalTransform(float3 position, Quat rotation, float3 scale)
{
    const float4x4 new_transform = float4x4::FromTRS(position, rotation, scale);
    SetLocalTransform(new_transform);
}

inline void Hachiko::ComponentTransform::SetLocalPosition(float3 new_position)
{
    SetLocalTransform(new_position, local_rotation, local_scale);
}

inline void Hachiko::ComponentTransform::SetLocalScale(float3 new_scale)
{
    SetLocalTransform(local_position, local_rotation, new_scale);
}

inline void Hachiko::ComponentTransform::SetLocalRotation(Quat new_rotation)
{
    SetLocalTransform(local_position, new_rotation, local_scale);
}

void Hachiko::ComponentTransform::SetLocalRotation(float3 rotation_angles)
{
    const float3 rotation = DegToRad(rotation_angles);
    SetLocalTransform(local_position, Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z).Normalized(), local_scale);
}

void Hachiko::ComponentTransform::SetLocalTransform(float4x4 new_transform)
{
    local_transform = new_transform;
    local_transform.Decompose(local_position, local_rotation, local_scale);
    local_rotation_euler = RadToDeg(local_rotation.ToEulerXYZ());

    UpdateGlobalTransformHierarchy();
}

void Hachiko::ComponentTransform::SetGlobalTransform(float4x4 transform)
{
    // Use parent global transform to get local transform	

    if (game_object->parent)
    {
        float4x4 parent_transform = game_object->parent->GetComponent<ComponentTransform>()->GetTransform();
        parent_transform.Inverse();
        SetLocalTransform(parent_transform * transform);
    }
    else
        SetLocalTransform(transform);
}

void Hachiko::ComponentTransform::UpdateGlobalTransformHierarchy()
{
    // Updates current transform based on parent and calls to update the children
    if (game_object->parent)
    {
        transform = game_object->parent->GetComponent<ComponentTransform>()->GetTransform() * local_transform;
    }
    else
    {
        transform = local_transform;
    }

    // Set transform as changed
    changed = true;

    // Update children
    for (GameObject* child : game_object->children)
    {
        child->GetComponent<ComponentTransform>()->UpdateGlobalTransformHierarchy();
    }
}

void Hachiko::ComponentTransform::SetPosition(float3 new_transform)
{
    transform.SetTranslatePart(new_transform);
    SetGlobalTransform(transform);
}

void Hachiko::ComponentTransform::Save(JsonFormatterValue j_component) const
{
    const JsonFormatterValue j_position = j_component["Position"];

    j_position[0] = local_position.x;
    j_position[1] = local_position.y;
    j_position[2] = local_position.z;

    const JsonFormatterValue j_rotation = j_component["Rotation"];

    j_rotation[0] = local_rotation.x;
    j_rotation[1] = local_rotation.y;
    j_rotation[2] = local_rotation.z;
    j_rotation[3] = local_rotation.w;

    const JsonFormatterValue j_scale = j_component["Scale"];

    j_scale[0] = local_scale.x;
    j_scale[1] = local_scale.y;
    j_scale[2] = local_scale.z;
}

void Hachiko::ComponentTransform::Load(JsonFormatterValue j_component)
{
    const JsonFormatterValue j_position = j_component["Position"];
    const JsonFormatterValue j_rotation = j_component["Rotation"];
    const JsonFormatterValue j_scale = j_component["Scale"];

    local_position = float3(j_position[0], j_position[1], j_position[2]);
    local_rotation = Quat(j_rotation[0], j_rotation[1], j_rotation[2], j_rotation[3]);
    local_scale = float3(j_scale[0], j_scale[1], j_scale[2]);

    SetLocalTransform(local_position, local_rotation, local_scale);
}

void Hachiko::ComponentTransform::DrawGui()
{
    static bool locked_scale = true;
    static bool debug_transforms = false;
    if (ImGui::CollapsingHeader("Local Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        float3 position = local_position;
        float3 scale = local_scale;
        float3 rotation = local_rotation_euler;
        if (ImGui::InputFloat3("position", &position[0]))
        {
            SetLocalPosition(position);
        }
        if (ImGui::InputFloat3("scale", &scale[0]))
        {
            SetLocalScale(scale);
        }
        if (ImGui::InputFloat3("rotation", &rotation[0]))
        {
            SetLocalRotation(rotation);
        }

        ImGui::Checkbox("Debug Transforms", &debug_transforms);
        if (debug_transforms)
        {
            ImGui::Separator();
            ImGui::Text("Local");
            for (int r = 0; r < 4; ++r)
            {
                auto row = local_transform.Row(r);
                ImGui::Text("%.2f, %.2f, %.2f, %.2f", row[0], row[1], row[2], row[3]);
            }

            ImGui::Separator();
            ImGui::Text("Global");
            for (int r = 0; r < 4; ++r)
            {
                auto row = transform.Row(r);
                ImGui::Text("%.2f, %.2f, %.2f, %.2f", row[0], row[1], row[2], row[3]);
            }
        }
    }
}
