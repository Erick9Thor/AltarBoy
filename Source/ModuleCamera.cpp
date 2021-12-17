#include "ModuleCamera.h"
#include "GameObject.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ModuleSceneManager.h"

#include "glew.h"
#include "Math/MathConstants.h"

#define MULT_CAM 15.0f
#define MOUSE_SPEED 10.0f

ModuleCamera::ModuleCamera()
{
	
}

ModuleCamera::~ModuleCamera()
{

}

bool ModuleCamera::Init() 
{
	GameObject* cameraGameObject = new GameObject();
	main_camera = (ComponentCamera*)cameraGameObject->CreateComponent(Component::Camera);

	cameraGameObject->GetComponent<ComponentTransform>()->SetPosition(float3(0.0f, 8.0f, 10.0f));
	cameraGameObject->GetComponent<ComponentTransform>()->LookAt(float3::zero);
	cameraGameObject->Update();
	
	return true;
}

update_status ModuleCamera::Update()
{
	CheckCameraControl();
	return UPDATE_CONTINUE;
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
	main_camera->SetResolution((float) screen_width, (float) screen_height);
}

void ModuleCamera::CheckCameraControl()
{

	// Keyboard for WASD movement -------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		int mouse_x, mouse_y;
		App->input->GetMouseMotion(mouse_x, mouse_y);

		RotationCamera(-(float)mouse_x * App->GetDeltaTime() * MOUSE_SPEED * DEGTORAD,
			(float)mouse_y * App->GetDeltaTime() * MOUSE_SPEED * DEGTORAD);
		MoveCamera((float)mouse_y, (float)mouse_y);
	}

	// Mouse ----------------------------
	if (App->input->MouseWheel())
	{
		int wheelX, wheelY;
		App->input->GetMouseWheel(wheelX, wheelY);

		ZoomCamera(wheelY);
	}
	if (App->input->GetKey(SDL_SCANCODE_LALT)) {
		int deltaX, deltaY;
		App->input->GetMouseMotion(deltaX, deltaY);

		OrbitCamera(deltaX * 1.5f, deltaY * 1.5f);
	}
	if (App->input->GetKey(SDL_SCANCODE_F)) {
		float distance = (main_camera->reference_point - main_camera->GetGameObject()->GetComponent<ComponentTransform>()->GetPosition()).Length();
		// Find the way to get the model gameObject
		// FocusCameraOnPosition(gameObject->GetComponent<C_Transform>()->GetPosition(), distance);
	}
}

void ModuleCamera::ZoomCamera(int zoom)
{
	ComponentTransform* transform = main_camera->GetGameObject()->GetComponent<ComponentTransform>();
	float distance = main_camera->reference_point.Distance(transform->GetPosition());
	vec newPos = transform->GetPosition() + transform->GetFwd() * zoom * distance * 0.05f;

	transform->SetPosition(newPos);
	main_camera->GetGameObject()->Update();
}

void ModuleCamera::OrbitCamera(float motion_x, float motion_y)
{
	ComponentTransform* transform = main_camera->GetGameObject()->GetComponent<ComponentTransform>();
	float3 vector = transform->GetPosition() - main_camera->reference_point;

	vector = Quat(transform->GetUp(), motion_x * 0.003f).Transform(vector);
	vector = Quat(transform->GetRight().Neg(), motion_y * 0.003f).Transform(vector);

	transform->SetPosition(vector + main_camera->reference_point);

	main_camera->GetGameObject()->GetComponent<ComponentTransform>()->LookAt(main_camera->reference_point);
	main_camera->GetGameObject()->Update();
}

void ModuleCamera::MoveCamera(float motion_x, float motion_y) {
	static float delta_time = App->GetDeltaTime();

	float multiplier = 1.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) multiplier *= 5.0f;
	if (App->input->GetKey(SDL_SCANCODE_LALT)) multiplier *= 0.5f;

	ComponentTransform* transform = main_camera->GetGameObject()->GetComponent<ComponentTransform>();
	float3 deltaRight = float3::zero, deltaUp = float3::zero, deltaFwd = float3::zero;

	if (App->input->GetKey(SDL_SCANCODE_W))
		deltaFwd += transform->GetFwd() * delta_time * MULT_CAM * multiplier;
	if (App->input->GetKey(SDL_SCANCODE_S))
		deltaFwd -= transform->GetFwd() * delta_time * MULT_CAM * multiplier;
	if (App->input->GetKey(SDL_SCANCODE_A))
		deltaRight += transform->GetRight() * delta_time * MULT_CAM * multiplier;
	if (App->input->GetKey(SDL_SCANCODE_D))
		deltaRight -= transform->GetRight() * delta_time * MULT_CAM * multiplier;
	if (App->input->GetKey(SDL_SCANCODE_Q))
		deltaUp += float3::unitY * delta_time * MULT_CAM * multiplier;
	if (App->input->GetKey(SDL_SCANCODE_E))
		deltaUp -= float3::unitY * delta_time * MULT_CAM * multiplier;

	transform->SetPosition(transform->GetPosition() + deltaFwd + deltaRight + deltaUp);
	main_camera->GetGameObject()->Update();
	main_camera->reference_point += deltaRight + deltaUp;
}

void ModuleCamera::FocusCameraOnTarget(const float3& target, float distance)
{
	ComponentTransform* transform = main_camera->GetGameObject()->GetComponent<ComponentTransform>();
	float3 v = transform->GetFwd().Neg();

	transform->SetPosition(target + (v * distance));
	main_camera->GetGameObject()->Update();

	main_camera->reference_point = target;
}

void ModuleCamera::RotationCamera(float motion_x, float motion_y) {
	ComponentTransform* transform = main_camera->GetGameObject()->GetComponent<ComponentTransform>();

	
	Quat yaw_quat = Quat::RotateY(motion_x);
	float3 newRight = yaw_quat * transform->GetRight();

	Quat pitch_quat = Quat::RotateAxisAngle(newRight, motion_y);
	float3 newUp = pitch_quat * yaw_quat * transform->GetUp();
	float3 newFwd = pitch_quat * yaw_quat * transform->GetFwd();

	transform->SetRotationAxis(newRight, newUp, newFwd);
	main_camera->GetGameObject()->Update();

	float distancetoReference = (main_camera->reference_point - transform->GetPosition()).Length();
	main_camera->reference_point = transform->GetPosition() + newFwd * distancetoReference;
}