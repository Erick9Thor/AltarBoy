#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleWindow.h"

ComponentCamera::ComponentCamera(GameObject* gameObject): Component(Component::Type::Camera, gameObject)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 1000.0f);

	auto screen_surface = App->window->getScreenSurface();
	
	frustum.SetHorizontalFovAndAspectRatio(hFov * DEGTORAD, (float)screen_surface->w / (float)screen_surface->h);

	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
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

float4x4 ComponentCamera::GetOpenGLViewMatrix() const
{
	static float4x4 view;
	view = frustum.ViewMatrix();
	view.Transpose();
	return view;
}

float4x4 ComponentCamera::GetOpenGLProjectionMatrix() const
{
	static float4x4 proj;
	proj = frustum.ProjectionMatrix();
	proj.Transpose();
	return proj;
}