#include "core/hepch.h"
#include "ComponentTransform2D.h"
#include "ComponentCanvas.h"
#include "ComponentButton.h"
#include "ComponentImage.h"
#include "ComponentCanvasRenderer.h"
#include "ComponentProgressBar.h"

#include "debugdraw.h"

Hachiko::ComponentTransform2D::ComponentTransform2D(GameObject* container) 
    : Component(Type::TRANSFORM_2D, container)
{
    Invalidate();
}

void Hachiko::ComponentTransform2D::DrawGui(){
    static bool debug_transforms = false;
    has_canvas = static_cast<bool>(game_object->GetComponent<ComponentCanvas>());

    // TODO: Draw panel for the inspector and update transform
    ImGui::PushID(this);

    if (ImGui::CollapsingHeader("2D Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        
        float3 ed_pos = position;
        if (ImGui::DragFloat3("Position", ed_pos.ptr(), 0.05f))
        {
            SetPosition(ed_pos);
        }

        if (!has_canvas)
        {
            float2 ed_siz = size;
            if (ImGui::DragFloat2("Size", ed_siz.ptr(), 0.05f))
            {
                SetSize(ed_siz);
            }
        }        

        float3 ed_sc = scale;
        if (ImGui::DragFloat2("Scale", ed_sc.ptr(), 0.05f))
        {
            SetScale(ed_sc.xy());
        }

        float3 ed_rot = rotation_euler;
        if (ImGui::DragFloat("Rotation (z)", &ed_rot.z, 0.05f))
        {
            SetRotation(ed_rot);
        }

        float2 ed_anch = anchor_pct_position;
        if (ImGui::DragFloat2("Anchor (Percent)", ed_anch.ptr(), 0.05f))
        {
            SetAnchor(ed_anch);
        }

        float2 ed_piv = pivot_pct_position;
        if (ImGui::DragFloat2("Pivot (Pefcent)", ed_piv.ptr(), 0.05f))
        {
            SetPivot(ed_piv);
        }

        ImGui::Checkbox("Debug Transform2D matrix", &debug_transforms);
        if (debug_transforms)
        {

            ImGui::Separator();
            ImGui::Text("Scaled Transform");
            const float4x4& scaled = GetGlobalScaledTransform();
            for (int r = 0; r < 4; ++r)
            {
                
                const float4& row = scaled.Row(r);
                ImGui::Text("%.2f, %.2f, %.2f, %.2f", row.x, row.y, row.z, row.w);
            }

            ImGui::Separator();
            ImGui::Text("Bounding 2D");
            ImGui::Text("%.2f %.2f, %.2f %.2f", aabb.minPoint.x, aabb.minPoint.y, aabb.maxPoint.x, aabb.maxPoint.y);
        }

    }

    ImGui::PopID();
}

void Hachiko::ComponentTransform2D::DebugDraw()
{
    // TODO: Draw debug information
    // Pivot position (center, color, w, h, d)
    dd::box(GetPivotScreenPosition(), dd::colors::Red, 0.1f, 0.1f, 0.f);

    ddVec3 rect_color = dd::colors::Blue;

    if (has_canvas)
    {
        rect_color = dd::colors::LightPink;
    }

    // Drawing box
    dd::box(global_transform.TranslatePart(), rect_color, size.x * scale.x, size.y * scale.y, 0.f);
}

void Hachiko::ComponentTransform2D::SetPosition(float3 new_position)
{
    position = new_position;
    Invalidate();
}

void Hachiko::ComponentTransform2D::SetSize(float2 new_size)
{
    size = new_size;
    Invalidate();
}

void Hachiko::ComponentTransform2D::SetScale(float2 new_scale)
{
    scale = float3(new_scale, 1.0f);
    Invalidate();
}

void Hachiko::ComponentTransform2D::SetRotation(Quat new_rotation)
{
    rotation = new_rotation;
    rotation_euler = RadToDeg(rotation.ToEulerXYZ());
    Invalidate();
}

void Hachiko::ComponentTransform2D::SetRotation(float3 new_rotation)
{
    rotation_euler = new_rotation;
    new_rotation = DegToRad(new_rotation);
    rotation = Quat::FromEulerXYZ(new_rotation.x, new_rotation.y, new_rotation.z);
    Invalidate();
}

void Hachiko::ComponentTransform2D::SetPivot(float2 new_pivot_position)
{
    float2 pivot_delta = new_pivot_position - pivot_pct_position;

    pivot_pct_position = new_pivot_position;

    // Update position to keep item in the same place
    position.x += pivot_delta.x * size.x;
    position.y += pivot_delta.y * size.y;
    
    Invalidate();
}

void Hachiko::ComponentTransform2D::SetAnchor(float2 new_anchor_position)
{
    anchor_pct_position = new_anchor_position;
    Invalidate();
}

float3 Hachiko::ComponentTransform2D::GetPivotOffsetFromParent() const
{

    float3 effective_position = position;
    GameObject* parent = game_object->parent;

    if (parent)
    {
        ComponentTransform2D* parent_transform = parent->GetComponent<ComponentTransform2D>();
        if (parent_transform)
        {
            float2& parent_size = parent_transform->size;
            effective_position.x = position.x + (parent_size.x * (anchor_pct_position.x - 0.5f));
            effective_position.y = position.y + (parent_size.y * (anchor_pct_position.y - 0.5f));
        }
    }

    return effective_position;
}

float3 Hachiko::ComponentTransform2D::GetPivotScreenPosition() const
{
    //HE_LOG("b");
    float3 screen_position = GetPivotOffsetFromParent();

    GameObject* parent = game_object->parent;

    while (parent)
    {
        ComponentTransform2D* parent_transform = parent->GetComponent<ComponentTransform2D>();
        if (!parent_transform)
        {
            break;
        }
        screen_position += parent_transform->GetPivotOffsetFromParent();
        parent = parent->parent;
    }
    return screen_position;
}

float4x4 Hachiko::ComponentTransform2D::GetGlobalTransform()
{
    UpdateTransforms();
    return global_transform;
}

float4x4 Hachiko::ComponentTransform2D::GetGlobalScaledTransform()
{
    UpdateTransforms();
    float4x4 scaled_transform = global_transform * float4x4::Scale(size.x, size.y, 1.f);
    return scaled_transform;
}

bool Hachiko::ComponentTransform2D::HasDependentComponents(GameObject* game_object) const
{
    bool found = game_object->GetComponent<ComponentCanvas>();
    found = found || game_object->GetComponent<ComponentCanvasRenderer>();
    
    found = found || game_object->GetComponent<ComponentImage>();  
    found = found || game_object->GetComponent<ComponentButton>();
    found = found || game_object->GetComponent<ComponentProgressBar>();

    for (GameObject* child : game_object->children)
    {
        found = HasDependentComponents(child);
        if (found)
        {
            break;
        }         
    }
    return found;
}

bool Hachiko::ComponentTransform2D::Intersects(const float2& mouse_pos) const
{
    return aabb.Contains(mouse_pos);
}

void Hachiko::ComponentTransform2D::Save(JsonFormatterValue j_component) const
{
    const JsonFormatterValue j_position = j_component["Position"];
    const JsonFormatterValue j_size = j_component["Size"];
    const JsonFormatterValue j_scale = j_component["Scale"];
    const JsonFormatterValue j_rotation = j_component["Rotation"];
    const JsonFormatterValue j_pivot = j_component["Pivot"];
    const JsonFormatterValue j_anchor = j_component["Anchor"];
    
    j_position[0] = position.x;
    j_position[1] = position.y;
    j_position[2] = position.z;

    j_size[0] = size.x;
    j_size[1] = size.y;

    j_scale[0] = scale.x;
    j_scale[1] = scale.y;

    j_rotation[0] = rotation.x;
    j_rotation[1] = rotation.y;
    j_rotation[2] = rotation.z;
    j_rotation[3] = rotation.w;

    j_pivot[0] = pivot_pct_position.x;
    j_pivot[1] = pivot_pct_position.y;

    j_anchor[0] = anchor_pct_position.x;
    j_anchor[1] = anchor_pct_position.y;
}

void Hachiko::ComponentTransform2D::Load(JsonFormatterValue j_component)
{
    const JsonFormatterValue j_position = j_component["Position"];
    const JsonFormatterValue j_size = j_component["Size"];
    const JsonFormatterValue j_scale = j_component["Scale"];
    const JsonFormatterValue j_rotation = j_component["Rotation"];
    const JsonFormatterValue j_pivot = j_component["Pivot"];
    const JsonFormatterValue j_anchor = j_component["Anchor"];
    
    SetAnchor(float2(j_anchor[0], j_anchor[1]));
    SetPivot(float2(j_pivot[0], j_pivot[1]));
    SetRotation(Quat(j_rotation[0], j_rotation[1], j_rotation[2], j_rotation[3]));
    SetScale(float2(j_scale[0], j_scale[1]));
    SetSize(float2(j_size[0], j_size[1]));
    SetPosition(float3(j_position[0], j_position[1], j_position[2]));
}

void Hachiko::ComponentTransform2D::Invalidate()
{
    dirty = true;

    for (GameObject* child : game_object->children)
    {
        ComponentTransform2D* transform = child->GetComponent<ComponentTransform2D>();
        if (transform)
        {
            transform->Invalidate();
        }
    }
}

void Hachiko::ComponentTransform2D::UpdateTransforms()
{
    if (dirty)
    {
        // Move center (pivot) relative to parent and anchors
        local_transform = float4x4::FromTRS(GetPivotOffsetFromParent(), rotation, scale);

        // Offset based on pivot
        float2 offset = (-pivot_pct_position + float2(0.5f)).Mul(size);
        local_transform = local_transform * float4x4::Translate(float3(offset, 0.0f));

        if (game_object->parent && game_object->parent->GetComponent<ComponentTransform2D>())
        {
            ComponentTransform2D* parent_transform = game_object->parent->GetComponent<ComponentTransform2D>();
            parent_transform->UpdateTransforms();
            global_transform = parent_transform->global_transform * local_transform;
        }
        else
        {
            global_transform = local_transform;
        }

        UpdateBoundingBox();
        UpdateUIComponents();

        dirty = false;
    }
}

void Hachiko::ComponentTransform2D::UpdateUIComponents()
{
}

void Hachiko::ComponentTransform2D::UpdateBoundingBox()
{
    aabb.SetNegativeInfinity();
    aabb.minPoint.Set(-(size.x / 2.f), -(size.y / 2.f));
    aabb.maxPoint.Set((size.x / 2.f), (size.y / 2.f));
    aabb.TransformAsAABB(global_transform);
}
