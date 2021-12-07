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
	GameObject* cameraGameObject = new GameObject();
	main_camera = (ComponentCamera*)cameraGameObject->CreateComponent(Component::Camera);

	cameraGameObject->GetComponent<ComponentTransform>()->SetPosition(float3(0.0f, 8.0f, 10.0f));
	cameraGameObject->GetComponent<ComponentTransform>()->LookAt(float3::zero);
	cameraGameObject->Update();
}

ModuleCamera::~ModuleCamera()
{
}

update_status ModuleCamera::Update()
{
	CheckCameraControl();
	return UPDATE_CONTINUE;
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
	main_camera->SetResolution(screen_width, screen_height);
}

void ModuleCamera::CheckCameraControl()
{

	if (App->input->MouseWheel())
	{
		int wheelX, wheelY;
		App->input->GetMouseWheel(wheelX, wheelY);

		ZoomCamera(wheelY);
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
