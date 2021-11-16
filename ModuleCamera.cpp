#include "ModuleCamera.h"
#include <glew.h>

ModuleCamera::ModuleCamera()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	frustum.SetPos(float3(0.0f, 1.0f, -2.0f));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	projectionGL = frustum.ProjectionMatrix().Transposed();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);

	viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);
}

ModuleCamera::~ModuleCamera()
{
}

void ModuleCamera::SetFOV(float fov)
{
	// frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);
}

void ModuleCamera::SetAspectRatio(float aspect_ratio)
{
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, aspect_ratio);
}

float4x4 ModuleCamera::getProjectionMatrix()
{
	return projectionGL;
}

float4x4 ModuleCamera::getViewMatrix()
{
	return viewGL;
}
