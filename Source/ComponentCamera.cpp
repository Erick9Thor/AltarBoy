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

	frustum.SetHorizontalFovAndAspectRatio(hFov * DEGTORAD, (float)DEFAULT_CAMERA_WIDTH / (float)DEFAULT_CAMERA_HEIGHT);

	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3x3::identity.WorldZ());
	frustum.SetUp(float3x3::identity.WorldY());

	frustum.GetPlanes(planes);
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::SetNearPlane(float distance)
{
	if (distance > 0 && distance < frustum.FarPlaneDistance())
		frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetFarPlane(float distance)
{
	if (distance > 0 && distance > frustum.NearPlaneDistance())
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetFOV(float fov)
{
	hFov = fov;
	frustum.SetHorizontalFovAndAspectRatio(hFov * DEGTORAD, frustum.AspectRatio());
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
	float verticalFov = frustum.VerticalFov();
	frustum.SetVerticalFovAndAspectRatio(verticalFov, width / height);
}
