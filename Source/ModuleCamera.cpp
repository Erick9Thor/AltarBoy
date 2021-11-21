#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "glew.h"
#include "Math/MathConstants.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);

	position = float3(0.0f, 8.0f, 10.0f);
	rotationMatrix = float3x3::FromEulerXYZ(DEGTORAD * -30.0f, DEGTORAD * 180.0f, 0.0f);

	auto screen_surface = App->window->screen_surface;

	SetAspectRatio(screen_surface->w, screen_surface->h);

	LookAt();
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	checkCameraControl();
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return false;
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
	aspect_ratio = (float)screen_width / (float)screen_height;
}

void ModuleCamera::LookAt()
{
	RefreshFov();

    frustum.SetPos(position);
    frustum.SetFront(rotationMatrix.WorldZ());
    frustum.SetUp(rotationMatrix.WorldY());
}

float4x4 ModuleCamera::GetGLView() const
{
    return float4x4(frustum.ViewMatrix()).Transposed();
}

float4x4 ModuleCamera::GetView() const
{
    return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetGLProjection() const
{
    return frustum.ProjectionMatrix().Transposed();
}

float4x4 ModuleCamera::GetProjection() const
{
    return frustum.ProjectionMatrix();
}

void ModuleCamera::checkCameraControl()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		float3x3 current_rotation =
			float3x3::RotateAxisAngle(rotationMatrix.WorldX(), deltaY * App->GetDeltaTime() * mouseSpeedForRotation * DEGTORAD) *
			float3x3::RotateAxisAngle(float3(0.0f, 1.0f, 0.0f), -deltaX * App->GetDeltaTime() * mouseSpeedForRotation * DEGTORAD);

		rotationMatrix = current_rotation.Mul(rotationMatrix);

		if (App->input->GetKey(SDL_SCANCODE_W))
			position += rotationMatrix.WorldZ() * App->GetDeltaTime() * CameraSpeed;
		if (App->input->GetKey(SDL_SCANCODE_S))
			position -= rotationMatrix.WorldZ() * App->GetDeltaTime() * CameraSpeed;
		if (App->input->GetKey(SDL_SCANCODE_A))
			position += rotationMatrix.WorldX() * App->GetDeltaTime() * CameraSpeed;
		if (App->input->GetKey(SDL_SCANCODE_D))
			position -= rotationMatrix.WorldX() * App->GetDeltaTime() * CameraSpeed;
		if (App->input->GetKey(SDL_SCANCODE_Q))
			position += rotationMatrix.WorldY() * App->GetDeltaTime() * CameraSpeed;
		if (App->input->GetKey(SDL_SCANCODE_E))
			position -= rotationMatrix.WorldY() * App->GetDeltaTime() * CameraSpeed;

	}
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		position += rotationMatrix.WorldX() * deltaX * App->GetDeltaTime() * mouseSpeedForMovement +
			rotationMatrix.WorldY() * deltaY * App->GetDeltaTime() * mouseSpeedForMovement;

	}
	else if (App->input->MouseWheel())
	{
		int wheelX, wheelY;
		App->input->GetMouseWheel(wheelX, wheelY);

		position += rotationMatrix.WorldZ() * wheelY * App->GetDeltaTime() * CameraSpeed;

	}
	
	LookAt();
}

void ModuleCamera::WindowResized(unsigned width, unsigned height)
{
	RefreshFov();
	LookAt();
}

void ModuleCamera::RefreshFov()
{
	frustum.SetVerticalFovAndAspectRatio(verticalFov, aspect_ratio);
}