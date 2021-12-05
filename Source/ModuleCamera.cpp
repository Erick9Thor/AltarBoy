#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"

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
	frustum.SetViewPlaneDistances(0.1f, 1000.0f);

	auto screen_surface = App->window->getScreenSurface();
	SetAspectRatio(screen_surface->w, screen_surface->h);

	frustum.SetVerticalFovAndAspectRatio(vertical_fov, aspect_ratio);

	position = float3(0.0f, 8.0f, 10.0f);
	rotation_matrix = float3x3::FromEulerXYZ(DEGTORAD * -30.0f, DEGTORAD * 180.0f, 0.0f);

	frustum.SetPos(position);
	frustum.SetFront(rotation_matrix.WorldZ());
	frustum.SetUp(rotation_matrix.WorldY());

	RefreshFov();

	return true;
}

update_status ModuleCamera::Update()
{
	CheckCameraControl();
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

	ImGui::Text("Set position");
	ImGui::DragFloat("position-X", &position.x, 1.0f, -25.0f, 25.0f, "%.2f");
	ImGui::DragFloat("position-Y", &position.y, 1.0f, -25.0f, 25.0f, "%.2f");
	ImGui::DragFloat("position-Z", &position.z, 1.0f, -25.0f, 25.0f, "%.2f");
	ImGui::Separator();

	UpdateCamera();
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
	aspect_ratio = (float)screen_width / (float)screen_height;
}

void ModuleCamera::UpdateCamera()
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

void ModuleCamera::CheckCameraControl()
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
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		position += rotation_matrix.WorldX() * deltaX * App->GetDeltaTime() * mouse_speed_movement +
			rotation_matrix.WorldY() * deltaY * App->GetDeltaTime() * mouse_speed_movement;

	}
	if (App->input->MouseWheel())
	{
		int wheelX, wheelY;
		App->input->GetMouseWheel(wheelX, wheelY);

		position += rotation_matrix.WorldZ() * wheelY * App->GetDeltaTime() * camera_speed;

	}
	if (App->input->GetKey(SDL_SCANCODE_LALT)) {
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		OrbitCamera(deltaX * 1.5f, deltaY * 1.5f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F)) {
		FocusOnModel(App->scene->scene_model);
	}
	
	UpdateCamera();
}

void ModuleCamera::RefreshFov()
{
	frustum.SetVerticalFovAndAspectRatio(vertical_fov, aspect_ratio);
}

void ModuleCamera::OrbitCamera(float motion_x, float motion_y)
{
	float3 vector = position - reference_point;

	Quat quat_y(rotation_matrix.WorldY(), motion_x * 0.003f);
	Quat quat_x(rotation_matrix.WorldX().Neg(), motion_y * 0.003f);

	vector = quat_x.Transform(vector);
	vector = quat_y.Transform(vector);
	
	position = vector + reference_point;

	float3 front = (float3(reference_point.x, reference_point.y, reference_point.z) - position).Normalized();
	float3 right = Cross(float3::unitY, front).Normalized();
	float3 up = Cross(front, right).Normalized();

	rotation_matrix = float3x3(right, up, front);
}

void ModuleCamera::FocusOnModel(Model* model)
{
	position = model->GetCenter() - rotation_matrix.WorldZ().Normalized() * model->GetDiameter().Length();
	UpdateCamera();
}