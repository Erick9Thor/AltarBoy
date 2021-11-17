#include "glew.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h";

#include "Math/MathConstants.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
    // TODO: Remove redundant variables when it works
    position = float3(0.0f, 0.0f, 1.0f);

    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    auto screen_surface = App->window->screen_surface;

    SetAspectRatio(screen_surface->w, screen_surface->h);
    SetHorizontalFov(90.0f);

    RefreshFov();
    frustum.SetViewPlaneDistances(0.1f, 100.0f);

    SetPosition(position);
    frustum.SetFront(float3::unitZ);
    frustum.SetUp(float3::unitY);

    LookAt(float3(0.0f, 0.0f, 0.0f));

    return true;
}

update_status ModuleCamera::PreUpdate()
{
    SetPosition(position);

    if (locked)
        LookAt(float3(0.0f, 0.0f, 0.0f));

    return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
    return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	checkCameraControl();

    return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
    return false;
}

void ModuleCamera::SetPosition(const float3& new_position)
{
    frustum.SetPos(position);
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
    aspect_ratio = (float)screen_width / (float)screen_height;
}

void ModuleCamera::SetHorizontalFov(float fov_deg)
{
    static const float deg_to_rad = pi / 180.0f;
    horizontal_fov = fov_deg * deg_to_rad;
}

float4x4 ModuleCamera::GetView() const
{
	return float4x4(frustum.ViewMatrix()).Transposed();
}

float4x4 ModuleCamera::GetProjection() const
{
	return frustum.ProjectionMatrix().Transposed();
}

void ModuleCamera::LookAt(const float3& look_position)
{
    float3 direction = look_position - frustum.Pos();
	rotationMatrix = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(rotationMatrix.MulDir(frustum.Front()).Normalized());
    frustum.SetUp(rotationMatrix.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::RefreshFov()
{
    frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ModuleCamera::WindowResized(unsigned int screen_width, unsigned int screen_height)
{
    SetAspectRatio(screen_width, screen_height);
    RefreshFov();
}

void ModuleCamera::checkCameraControl()
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

	if (change) {
		frustum.SetVerticalFovAndAspectRatio(DEGTORAD * 45.0f, frustum.AspectRatio());

		frustum.SetPos(position);
		frustum.SetFront(rotationMatrix.WorldZ());
		frustum.SetUp(rotationMatrix.WorldY());
	}
}