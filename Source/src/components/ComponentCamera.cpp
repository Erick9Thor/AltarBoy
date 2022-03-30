#include "core/hepch.h"

#include "modules/ModuleSceneManager.h"
#include "modules/ModuleCamera.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"

#include <debugdraw.h>

Hachiko::ComponentCamera::ComponentCamera(GameObject* container) :
    Component(Type::CAMERA, container),
    horizontal_fov(65.0f)
{
    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    frustum.SetViewPlaneDistances(0.1f, 1000.0f);

    frustum.SetHorizontalFovAndAspectRatio(DegToRad(horizontal_fov), static_cast<float>(DEFAULT_CAMERA_WIDTH) / static_cast<float>(DEFAULT_CAMERA_HEIGHT));

    frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
    frustum.SetFront(float3x3::identity.WorldZ());
    frustum.SetUp(float3x3::identity.WorldY());

    frustum.GetPlanes(planes);

    ComponentCamera::OnTransformUpdated();
}

Hachiko::ComponentCamera::~ComponentCamera()
{
    if (game_object->scene_owner)
    {
        if (game_object->scene_owner->GetCullingCamera() == this)
        {
            game_object->scene_owner->SetCullingCamera(App->camera->GetMainCamera());
        }
    }
}

void Hachiko::ComponentCamera::DebugDraw()
{
    if (draw_frustum)
    {
        float4x4 inverse_clip;
        float4x4 matrix = GetProjectionMatrix() * GetViewMatrix();
        matrix.Inverse();
        dd::frustum(matrix, dd::colors::Yellow);
    }
}

void Hachiko::ComponentCamera::SetNearPlane(float distance)
{
    SetPlaneDistances(distance, frustum.FarPlaneDistance());
}

void Hachiko::ComponentCamera::SetFarPlane(float distance)
{
    SetPlaneDistances(frustum.NearPlaneDistance(), distance);
}

void Hachiko::ComponentCamera::SetPlaneDistances(const float near_distance, const float far_distance)
{
    frustum.SetViewPlaneDistances(near_distance, far_distance);
    frustum.GetPlanes(planes);
}

void Hachiko::ComponentCamera::SetHorizontalFov(float fov_deg)
{
    horizontal_fov = fov_deg;
    frustum.SetHorizontalFovAndAspectRatio(DegToRad(horizontal_fov), frustum.AspectRatio());
    frustum.GetPlanes(planes);
}

float4x4 Hachiko::ComponentCamera::GetViewMatrix(const bool transpose) const
{
    if (transpose)
    {
        return float4x4(frustum.ViewMatrix()).Transposed();
    }
    return float4x4(frustum.ViewMatrix());
}

float4x4 Hachiko::ComponentCamera::GetProjectionMatrix(const bool transpose) const
{
    if (transpose)
    {
        return frustum.ProjectionMatrix().Transposed();
    }
    return frustum.ProjectionMatrix();
}

void Hachiko::ComponentCamera::OnTransformUpdated()
{
    ComponentTransform* transform = game_object->GetTransform();

    frustum.SetFrame(transform->GetPosition(), transform->GetFront(), transform->GetUp());
    frustum.GetPlanes(planes);
}

void Hachiko::ComponentCamera::SetResolution(unsigned width, unsigned height)
{
    if (resolution_x != width || resolution_y != height)
    {
        resolution_x = width;
        resolution_y = height;
        frustum.SetHorizontalFovAndAspectRatio(DegToRad(horizontal_fov), (static_cast<float>(width) / static_cast<float>(height)));
    }
}

void Hachiko::ComponentCamera::GetResolution(unsigned& width, unsigned& height) const
{
    width = resolution_x;
    height = resolution_y;
}

LineSegment Hachiko::ComponentCamera::RayCast(float x, float y) const
{
    return frustum.UnProjectLineSegment(x, y);
}

void Hachiko::ComponentCamera::Save(YAML::Node& node) const
{
    YAML::Node node_frustum = node[FRUSTUM];
    node_frustum[NEAR_DISTANCE] = frustum.NearPlaneDistance();
    node_frustum[FAR_DISTANCE] = frustum.FarPlaneDistance();
    node_frustum[FOV] = horizontal_fov;
    node_frustum[CAMERA_POSITION] = frustum.Pos();
    node_frustum[CAMERA_FRONT] = frustum.Front();
    node_frustum[CAMERA_UP] = frustum.Up();
}

void Hachiko::ComponentCamera::Load(const YAML::Node& node)
{
    const YAML::Node frustum_node = node[FRUSTUM];

    SetNearPlane(frustum_node[NEAR_DISTANCE].as<float>());
    SetFarPlane(frustum_node[FAR_DISTANCE].as<float>());
    SetHorizontalFov(frustum_node[FOV].as<float>());
    frustum.SetPos(frustum_node[CAMERA_POSITION].as<vec>());
    frustum.SetFront(frustum_node[CAMERA_FRONT].as<vec>());
    frustum.SetUp(frustum_node[CAMERA_UP].as<vec>());
}

void Hachiko::ComponentCamera::DrawGui()
{
    ImGui::PushID(this);
    static bool debug_data = false;

    if (ImGuiUtils::CollapsingHeader(game_object, this, "Camera"))
    {
        ImGui::Checkbox("Draw Frustum", &draw_frustum);

        float planes[2] = {frustum.NearPlaneDistance(), frustum.FarPlaneDistance()};
        if (ImGui::InputFloat2("N & F", &planes[0]))
        {
            SetPlaneDistances(planes[0], planes[1]);
        }
        if (ImGui::SliderFloat("H. Fov", &horizontal_fov, 30.f, 180.f))
        {
            SetHorizontalFov(horizontal_fov);
        }
        if (ImGui::Button("Culling Camera"))
        {
            App->scene_manager->GetActiveScene()->SetCullingCamera(this);
        }
        ImGui::Checkbox("Debug", &debug_data);
        if (debug_data)
        {
            ImGui::Separator();
            ImGui::Text("Fov (H, V): %.2f, %.2f", RadToDeg(frustum.HorizontalFov()), RadToDeg(frustum.VerticalFov()));
            ImGui::Text("Aspect Ratio: %.2f", frustum.AspectRatio());
        }
    }
    ImGui::PopID();
}
