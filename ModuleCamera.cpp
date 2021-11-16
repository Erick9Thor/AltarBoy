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

	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed();
		//Send the frustum projection matrix to OpenGL
		// direct mode would be:
		glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);
}

ModuleCamera::~ModuleCamera()
{
}

void ModuleCamera::SetFOV(float fov)
{

}


void ModuleCamera::SetAspectRatio(float aspect_ratio)
{

}