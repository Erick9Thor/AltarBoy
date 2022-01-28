#include "../Application.h"
#include "../Modules/ModuleWindow.h"
#include "../Modules/ModuleRender.h"
#include "../Utils/Logger.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "debugdraw.h"

#include <glew.h>
#include <imgui.h>

ComponentCamera::ComponentCamera(GameObject* container)
	: Component(Component::Type::Camera, container)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 1000.0f);

	horizontal_fov = 65.0f;
	frustum.SetHorizontalFovAndAspectRatio(DegToRad(horizontal_fov), (float) DEFAULT_CAMERA_WIDTH / (float) DEFAULT_CAMERA_HEIGHT);

	frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
	frustum.SetFront(float3x3::identity.WorldZ());
	frustum.SetUp(float3x3::identity.WorldY());

	frustum.GetPlanes(planes);

	OnTransformUpdated();
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::DebugDraw()
{
	if (draw_frustum) {
		float4x4 inverse_clip;
		float4x4 matrix = GetProjectionMatrix() * GetViewMatrix();
		matrix.Inverse();
		dd::frustum(matrix, dd::colors::Yellow);
	}
}

void ComponentCamera::SetNearPlane(float distance)
{
	SetPlaneDistances(distance, frustum.FarPlaneDistance());
}

void ComponentCamera::SetFarPlane(float distance)
{
	SetPlaneDistances(frustum.NearPlaneDistance(), distance);
}

void ComponentCamera::SetPlaneDistances(const float near_distance, const float far_distance)
{
	frustum.SetViewPlaneDistances(near_distance, far_distance);
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetFOV(float fov)
{
	horizontal_fov = fov;
	frustum.SetHorizontalFovAndAspectRatio(DegToRad(horizontal_fov), frustum.AspectRatio());
	frustum.GetPlanes(planes);
}

float4x4 ComponentCamera::GetViewMatrix(const bool transpose) const
{
	if (transpose)
		return float4x4(frustum.ViewMatrix()).Transposed();
	return float4x4(frustum.ViewMatrix());
}

float4x4 ComponentCamera::GetProjectionMatrix(const bool transpose) const
{
	if (transpose)
		return frustum.ProjectionMatrix().Transposed();
	return frustum.ProjectionMatrix();
}

void ComponentCamera::OnTransformUpdated()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();

	frustum.SetFrame(transform->GetPosition(), transform->GetFwd(), transform->GetUp());
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetResolution(unsigned width, unsigned height)
{
	if (resolution_x != width || resolution_y != height)
	{
		resolution_x = width;
		resolution_y = height;
		frustum.SetHorizontalFovAndAspectRatio(DegToRad(horizontal_fov), ((float) width / (float) height));
	}	
}

void ComponentCamera::GetResolution(unsigned& width, unsigned& height) const
{
	width = resolution_x;
	height = resolution_y;
}

void ComponentCamera::DrawGui()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Draw Frustum", &draw_frustum);
		
		float planes[2] = {frustum.NearPlaneDistance(), frustum.FarPlaneDistance()};
		if (ImGui::SliderFloat2("N & F", &planes[0], 0.1f, 500.0f))
			SetPlaneDistances(planes[0], planes[1]);
	}
}

void ComponentCamera::Save(JsonFormaterValue j_component) const
{
	JsonFormaterValue j_frustum = j_component["Frustrum"];
	j_frustum["NearDistance"] = frustum.NearPlaneDistance();
	j_frustum["FarDistance"] = frustum.FarPlaneDistance();
	j_frustum["Fov"] = horizontal_fov;

	JsonFormaterValue j_pos = j_frustum["Pos"];
	j_pos[0] = frustum.Pos().x;
	j_pos[1] = frustum.Pos().y;
	j_pos[2] = frustum.Pos().z;

	JsonFormaterValue j_front = j_frustum["Front"];
	j_front[0] = frustum.Front().x;
	j_front[1] = frustum.Front().y;
	j_front[2] = frustum.Front().z;

	JsonFormaterValue j_up = j_frustum["Up"];
	j_up[0] = frustum.Up().x;
	j_up[1] = frustum.Up().y;
	j_up[2] = frustum.Up().z;
}

void ComponentCamera::Load(JsonFormaterValue j_component)
{
	JsonFormaterValue j_frustum = j_component["Frustrum"];

	SetNearPlane(j_frustum["NearDistance"]);
	SetFarPlane(j_frustum["FarDistance"]);
	SetFOV(j_frustum["Fov"]);

	JsonFormaterValue j_pos = j_frustum["Pos"];
	JsonFormaterValue j_front = j_frustum["Front"];
	JsonFormaterValue j_up = j_frustum["Up"];

	frustum.SetPos(float3(j_pos[0], j_pos[1], j_pos[2]));
	frustum.SetFront(float3(j_front[0], j_front[1], j_front[2]));
	frustum.SetUp(float3(j_up[0], j_up[1], j_up[2]));
}

LineSegment ComponentCamera::RayCast(float x, float y)
{
	return frustum.UnProjectLineSegment(x, y);
}
