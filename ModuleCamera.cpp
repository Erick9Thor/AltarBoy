#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleInput.h"
#include "SDL.h"

ModuleCamera::ModuleCamera()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
}

ModuleCamera::~ModuleCamera()
{
}

void ModuleCamera::ViewProjectionMatrix()
{
	
	frustum.SetVerticalFovAndAspectRatio(DEGTORAD * 45.0f, frustum.AspectRatio());

	frustum.SetPos(position);
	frustum.SetFront(rotationMatrix.WorldZ());
	frustum.SetUp(rotationMatrix.WorldY());

	view = float4x4(frustum.ViewMatrix());
	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::WindowResized(unsigned width, unsigned height)
{
	SetAspectRatio(width / height);

	ViewProjectionMatrix();
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

bool ModuleCamera::Init()
{
	// Initial camera position
	position = float3(3.0f, 4.0f, 8.0f);

	rotationMatrix = float3x3::FromEulerXYZ(DEGTORAD * -30.0f, DEGTORAD * 180.0f, 0.0f);

	SetAspectRatio(SCREEN_WIDTH / SCREEN_HEIGHT);

	ViewProjectionMatrix();

	return true;
}

update_status ModuleCamera::Update()
{
	bool change = false;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		float3x3 rotationDeltaMatrix =
			float3x3::RotateAxisAngle(rotationMatrix.WorldX(), deltaY * 0.25f * DEGTORAD) *
			float3x3::RotateAxisAngle(float3(0.0f, 1.0f, 0.0f), -deltaX * 0.25f * DEGTORAD);

		rotationMatrix = rotationDeltaMatrix * rotationMatrix;

		LOG("%i %i", deltaX, deltaY);

		float speed = 0.1f;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
			speed *= 2.0f;

		if (App->input->GetKey(SDL_SCANCODE_W))
			position += rotationMatrix.WorldZ() * speed;
		if (App->input->GetKey(SDL_SCANCODE_S))
			position -= rotationMatrix.WorldZ() * speed;
		if (App->input->GetKey(SDL_SCANCODE_A))
			position += rotationMatrix.WorldX() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D))
			position -= rotationMatrix.WorldX() * speed;
		if (App->input->GetKey(SDL_SCANCODE_Q))
			position += rotationMatrix.WorldY() * speed;
		if (App->input->GetKey(SDL_SCANCODE_E))
			position -= rotationMatrix.WorldY() * speed;

		change = true;
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		position += rotationMatrix.WorldX() * deltaX * 0.1f + rotationMatrix.WorldY() * deltaY * 0.1f;

		change = true;
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_W))
		{
			position += float3(0.0f, 0.0f, -0.1f);
			change = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_S))
		{
			position += float3(0.0f, 0.0f, 0.1f);
			change = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_A))
		{
			position += float3(-0.1f, 0.0f, 0.0f);
			change = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D))
		{
			position += float3(0.1f, 0.0f, 0.0f);
			change = true;
		}
	}

	if (change)
		ViewProjectionMatrix();

	return UPDATE_CONTINUE;
}
