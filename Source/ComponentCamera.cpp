#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "ComponentTransform.h"

#define DEFAULT_CAMERA_WIDTH 1920
#define DEFAULT_CAMERA_HEIGHT 1080

ComponentCamera::ComponentCamera(GameObject* container): Component(Component::Type::Camera, container)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 1000.0f);

	horizontal_fov = 90.0f;
	frustum.SetHorizontalFovAndAspectRatio(horizontal_fov * to_rad, (float)DEFAULT_CAMERA_WIDTH / (float)DEFAULT_CAMERA_HEIGHT);

	frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
	frustum.SetFront(float3x3::identity.WorldZ());
	frustum.SetUp(float3x3::identity.WorldY());

	frustum.GetPlanes(planes);
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::SetNearPlane(float distance)
{
	frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetFarPlane(float distance)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetFOV(float fov)
{
	horizontal_fov = fov;
	frustum.SetHorizontalFovAndAspectRatio(horizontal_fov * to_rad, frustum.AspectRatio());
	frustum.GetPlanes(planes);
}

float4x4 ComponentCamera::GetViewMatrix(const bool transpose) const
{
	static float4x4 view;
	view = frustum.ViewMatrix();
	if (transpose) {
		view.Transpose();
	}
	return view;
}

float4x4 ComponentCamera::GetProjectionMatrix(const bool transpose) const
{
	static float4x4 proj;
	proj = frustum.ProjectionMatrix();
	if (transpose) {
		proj.Transpose();
	}
	return proj;
}

void ComponentCamera::OnTransformUpdated()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
	
	frustum.SetFrame(transform->GetPosition(), transform->GetFwd(), transform->GetUp());
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetResolution(float width, float height)
{
	float vertical_fov = frustum.VerticalFov();
	frustum.SetVerticalFovAndAspectRatio(vertical_fov, width / height);
}

void ComponentCamera::DrawGui()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}