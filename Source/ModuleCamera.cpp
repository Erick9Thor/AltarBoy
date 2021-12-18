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

update_status ModuleCamera::Update(const float delta)
{
	Controller(delta);
	return UPDATE_CONTINUE;
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
	main_camera->SetResolution((float) screen_width, (float) screen_height);
}


void ModuleCamera::Controller(const float delta)
{
	static const float zoom_speed = 3.0f;
	static const float rot_speed = 2.0f;
	// Keyboard for WASD movement -------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		int moved_x, moved_y;
		App->input->GetMouseDelta(moved_x, moved_y);

		RotationCamera(-(float)moved_x * delta * rot_speed, (float)moved_y * delta * rot_speed);
		MovementController(delta);
	}

	// Mouse ----------------------------
	int scrolled_y = App->input->GetScrollDelta();
	if (scrolled_y != 0)
		ZoomCamera(zoom_speed * -scrolled_y);

	if (App->input->GetKey(SDL_SCANCODE_LALT)) {
		int moved_x, moved_y;
		App->input->GetMouseDelta(moved_x, moved_y);
		OrbitCamera(-rot_speed * (float)moved_y * delta, -rot_speed * (float)moved_x * delta);

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

void ModuleCamera::MovementController(const float delta) {

	static const float move_speed = 15.0f;
	static const float speed_modifier = 2.0f;

	float effective_speed = move_speed;
	if (App->input->GetKeyMod(KMOD_SHIFT))
		effective_speed *= speed_modifier;

	ComponentTransform* transform = main_camera->GetGameObject()->GetComponent<ComponentTransform>();
	float3 deltaRight = float3::zero, deltaUp = float3::zero, deltaFwd = float3::zero;

	if (App->input->GetKey(SDL_SCANCODE_W))
		deltaFwd += transform->GetFwd() * delta * effective_speed;
	if (App->input->GetKey(SDL_SCANCODE_S))
		deltaFwd -= transform->GetFwd() * delta * effective_speed;
	if (App->input->GetKey(SDL_SCANCODE_A))
		deltaRight += transform->GetRight() * delta * effective_speed;
	if (App->input->GetKey(SDL_SCANCODE_D))
		deltaRight -= transform->GetRight() * delta * effective_speed;
	if (App->input->GetKey(SDL_SCANCODE_Q))
		deltaUp += float3::unitY * delta * effective_speed;
	if (App->input->GetKey(SDL_SCANCODE_E))
		deltaUp -= float3::unitY * delta * effective_speed;

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