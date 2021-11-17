#include "ModuleCamera.h"
#include <glew.h>

ModuleCamera::ModuleCamera()
{

	float initial_aspect_ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, initial_aspect_ratio);

	frustum.SetPos(float3(0.0f, 4.0f, 8.0f));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	projectionGL = frustum.ProjectionMatrix().Transposed();
	
	viewGL = float4x4(frustum.ViewMatrix()).Transposed();

	rotationMatrix = float3x3::FromEulerXYZ(DEGTORAD * -30.0f, DEGTORAD * 180.0f, 0.0f);
}

ModuleCamera::~ModuleCamera()
{
}

void ModuleCamera::SetFOV(float fov)
{
	frustum.SetHorizontalFovAndAspectRatio(fov, frustum.AspectRatio());
}

void ModuleCamera::SetAspectRatio(float aspect_ratio)
{
	frustum.SetVerticalFovAndAspectRatio(frustum.HorizontalFov(), aspect_ratio);
}

void ModuleCamera::SetPlaneDistances(float nearDistance, float farDistance) {
	frustum.SetViewPlaneDistances(nearDistance, farDistance);
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}

float4x4 ModuleCamera::getProjectionMatrix()
{
	return projectionGL;
}

float4x4 ModuleCamera::getViewMatrix()
{
	return viewGL;
}
