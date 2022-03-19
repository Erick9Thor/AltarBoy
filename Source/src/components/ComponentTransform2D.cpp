#include "core/hepch.h"
#include "ComponentTransform2D.h"
#include "ComponentCanvas.h"
#include "ComponentButton.h"
#include "ComponentImage.h"
#include "ComponentCanvasRenderer.h"

#include "debugdraw.h"

Hachiko::ComponentTransform2D::ComponentTransform2D(GameObject* container) 
    : Component(Type::TRANSFORM_2D, container)
{
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::DrawGui(){
    static bool debug_transforms = false;

    // TODO: Draw panel for the inspector and update transform
    ImGui::PushID(this);

    if (ImGui::CollapsingHeader("2D Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        
        float3 ed_pos = position;
        if (ImGui::DragFloat3("Position", ed_pos.ptr(), 0.05f))
        {
            SetPosition(ed_pos);
        }

        if (!canvas)
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
                auto row = global_transform.Row(r);
                ImGui::Text("%.2f, %.2f, %.2f, %.2f", row[0], row[1], row[2], row[3]);
            }
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

    if (canvas)
    {
        rect_color = dd::colors::LightPink;
    }

    // Drawing box
    dd::box(global_transform.TranslatePart(), rect_color, size.x * scale.x, size.y * scale.y, 0.f);
}

void Hachiko::ComponentTransform2D::SetPosition(float3 new_position)
{
    position = new_position;
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::SetSize(float2 new_size)
{
    size = new_size;
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::SetScale(float2 new_scale)
{
    scale = float3(new_scale, 1.0f);
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::SetRotation(Quat new_rotation)
{
    rotation = new_rotation;
    rotation_euler = RadToDeg(rotation.ToEulerXYZ());
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::SetRotation(float3 new_rotation)
{
    rotation_euler = new_rotation;
    new_rotation = DegToRad(new_rotation);
    rotation = Quat::FromEulerXYZ(new_rotation.x, new_rotation.y, new_rotation.z);
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::SetPivot(float2 new_pivot_position)
{
    float2 pivot_delta = new_pivot_position - pivot_pct_position;

    pivot_pct_position = new_pivot_position;

    // Update position to keep item in the same place
    position.x += pivot_delta.x * size.x;
    position.y += pivot_delta.y * size.y;
    
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::SetAnchor(float2 new_anchor_position)
{
    anchor_pct_position = new_anchor_position;
    UpdateHierarchy();
}

void Hachiko::ComponentTransform2D::SetCanvas(ComponentCanvas* owner_canvas)
{
    canvas = owner_canvas;
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
        parent = game_object->parent->parent;
    }
    return screen_position;
}

float4x4 Hachiko::ComponentTransform2D::GetGlobalScaledTransform() const
{
    float4x4 scaled_transform = global_transform * float4x4::Scale(size.x, size.y, 1.f);
    return scaled_transform;
}

void Hachiko::ComponentTransform2D::UpdateHierarchy()
{
    UpdateTransforms();

    for (GameObject* child : game_object->children)
    {
        ComponentTransform2D* transform_2d = child->GetComponent<ComponentTransform2D>();
        if (transform_2d)
        {
            transform_2d->UpdateHierarchy();
        }
    }
}

bool Hachiko::ComponentTransform2D::HasDependentComponents(GameObject* game_object) const
{
    bool found = game_object->GetComponent<ComponentCanvas>();
    found = found || game_object->GetComponent<ComponentCanvasRenderer>();
    
    found = found || game_object->GetComponent<ComponentImage>();  
    found = found || game_object->GetComponent<ComponentButton>();

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

void Hachiko::ComponentTransform2D::UpdateTransforms()
{
    // Move center (pivot) relative to parent and anchors
    local_transform = float4x4::FromTRS(GetPivotOffsetFromParent(), rotation, scale);
    
    // Offset based on pivot
    float2 offset = (-pivot_pct_position + float2(0.5f)).Mul(size);
    local_transform = local_transform * float4x4::Translate(float3(offset, 0.0f));

    if (game_object->parent && game_object->parent->GetComponent<ComponentTransform2D>())
    {
        ComponentTransform2D* parent_transform = game_object->parent->GetComponent<ComponentTransform2D>();
        global_transform = parent_transform->global_transform * local_transform;
    }
    else
    {
        global_transform = local_transform;
    }

    // TODO: Add transform updated refresh to ui components if necessary
}

void Hachiko::ComponentTransform2D::UpdateUIComponents()
{
}
