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
	frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);

	position = float3(0.0f, 8.0f, 10.0f);
	rotation_matrix = float3x3::FromEulerXYZ(DEGTORAD * -30.0f, DEGTORAD * 180.0f, 0.0f);

	auto screen_surface = App->window->getScreenSurface();

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

void ModuleCamera::DrawGui()
{
	ImGui::Text("Variables");
	if (ImGui::DragFloat("Vertical FOV", &initial_vertical_fov, 1.0f, 10.0f, 160.0f, "%.2f")) {
		if (aspect_ratio >= 1)
			vertical_fov = DEGTORAD * initial_vertical_fov;
		else
			vertical_fov = math::Atan(math::Tan(DEGTORAD * initial_vertical_fov) / aspect_ratio);
	}
	ImGui::DragFloat("Near distance near Z:", &near_plane_distance, 1.0f, 0.1f, 5.0f, "%.2f");
	ImGui::DragFloat("Far distance Z", &far_plane_distance, 5.0f, 6.0f, 400.0f, "%.2f");

	ImGui::Text("Set position");
	ImGui::DragFloat("position-X", &position.x, 1.0f, -25.0f, 25.0f, "%.2f");
	ImGui::DragFloat("position-Y", &position.y, 1.0f, -25.0f, 25.0f, "%.2f");
	ImGui::DragFloat("position-Z", &position.z, 1.0f, -25.0f, 25.0f, "%.2f");
	ImGui::Separator();

	LookAt();
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
	aspect_ratio = (float)screen_width / (float)screen_height;
}

void ModuleCamera::LookAt()
{
	RefreshFov();

    frustum.SetPos(position);
    frustum.SetFront(rotation_matrix.WorldZ());
    frustum.SetUp(rotation_matrix.WorldY());
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

		if (App->input->GetKey(SDL_SCANCODE_LALT))
		{
			position += rotation_matrix.WorldZ() * deltaY * App->GetDeltaTime() * mouse_speed_rotation;
		}
		else {
			static float delta_time = App->GetDeltaTime();

			rotation_matrix = float3x3::RotateAxisAngle(float3::unitY, -deltaX * delta_time * mouse_speed_rotation * DEGTORAD) *
				rotation_matrix * float3x3::RotateAxisAngle(float3::unitX, deltaY * delta_time * mouse_speed_rotation * DEGTORAD);

			float multiplier = 1.0f;
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
				multiplier = 3.0f;

			if (App->input->GetKey(SDL_SCANCODE_W))
				position += rotation_matrix.WorldZ() * delta_time * camera_speed * multiplier;
			if (App->input->GetKey(SDL_SCANCODE_S))
				position -= rotation_matrix.WorldZ() * delta_time * camera_speed * multiplier;
			if (App->input->GetKey(SDL_SCANCODE_A))
				position += rotation_matrix.WorldX() * delta_time * camera_speed * multiplier;
			if (App->input->GetKey(SDL_SCANCODE_D))
				position -= rotation_matrix.WorldX() * delta_time * camera_speed * multiplier;
			if (App->input->GetKey(SDL_SCANCODE_Q))
				position += rotation_matrix.WorldY() * delta_time * camera_speed * multiplier;
			if (App->input->GetKey(SDL_SCANCODE_E))
				position -= rotation_matrix.WorldY() * delta_time * camera_speed * multiplier;
		}
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		position += rotation_matrix.WorldX() * deltaX * App->GetDeltaTime() * mouse_speed_movement +
			rotation_matrix.WorldY() * deltaY * App->GetDeltaTime() * mouse_speed_movement;

	}
	else if (App->input->MouseWheel())
	{
		int wheelX, wheelY;
		App->input->GetMouseWheel(wheelX, wheelY);

		position += rotation_matrix.WorldZ() * wheelY * App->GetDeltaTime() * camera_speed;

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
	frustum.SetVerticalFovAndAspectRatio(vertical_fov, aspect_ratio);
}