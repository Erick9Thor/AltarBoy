#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include "components/ComponentTransform.h"
#include "modules/ModuleSceneManager.h"

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
	, _movement_speed(0.0f)
	, _dash_distance(0.0f)
	, _dash_duration(0.0f)
	, _is_dashing(false)
	, _dash_progress(0.0f)
	, _dash_start(math::float3::zero)
	, _rotation_speed(0.0f)
	, _is_falling(false)
	, _original_y(0.0f)
	, _speed_y(0.0f)
	, _starting_position(math::float3::zero)
{}

void Hachiko::Scripting::PlayerController::OnAwake()
{
	_dash_distance = 1.0f;
	_dash_duration = 0.10f;
	_movement_speed = 10.0f;
	_rotation_speed = 2.5f;

	_original_y = game_object->GetTransform()->GetGlobalPosition().y;
	_speed_y = 0.0f;
	
	_starting_position = math::float3(0.0f, 1.0f, 0.0f);
	HE_LOG("OnAwake %f", _movement_speed);
}

void Hachiko::Scripting::PlayerController::OnStart()
{
	game_object->GetTransform()->SetGlobalPosition(_starting_position);
}

void Hachiko::Scripting::PlayerController::OnUpdate()
{
	// Ignore the inputs if engine camera input is taken:
	if (Input::GetMouseButton(Input::MouseButton::RIGHT))
	{
		return;
	}

	ComponentTransform* transform = game_object->GetTransform();
	
	math::float3 current_position = transform->GetGlobalPosition();
	math::float3 current_front = transform->GetFront();
	math::float3 current_right = transform->GetRight();

	float delta_time = Time::DeltaTime();
	float velocity = _movement_speed * delta_time;

	GameObject* go = SceneManagement::Raycast(current_position - math::float3(0.0f, 1.0f, 0.0f), current_position);
	if (go == nullptr || go->name == "Player") 
	{
		_is_falling = true;
	}

	if (go->name == "Goal")
	{
		SceneManagement::SwitchScene("Assets/Scenes/menu.scene"); //Change this to the victory scene or add a victory effect
	}

	if (!_is_dashing && !_is_falling)
	{
		if (Input::GetKey(Input::KeyCode::KEY_W))
		{
			current_position -= math::float3(0, 0, 1) * velocity;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_S))
		{
			current_position += math::float3(0, 0, 1) * velocity;
		}

		if (Input::GetKey(Input::KeyCode::KEY_D))
		{
			current_position += math::float3(1, 0, 0) * velocity;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_A))
		{
			current_position -= math::float3(1, 0, 0) * velocity;
		}

		if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
		{
			_is_dashing = true;
			_dash_progress = 0.0f;
			_dash_start = current_position;
		}
	}

	if (_is_dashing)
	{
		_dash_progress += delta_time / _dash_duration;
		_dash_progress = _dash_progress > 1.0f ? 1.0f : _dash_progress;

		_is_dashing = (_dash_progress < 1.0f);

		math::float3 _dash_end = current_position + 
			current_front * _dash_distance;

		current_position = math::float3::Lerp(_dash_start, _dash_end, 
			_dash_progress);
	}

	if (_is_falling)
	{
		_speed_y += 25.0f * delta_time;
		current_position.y -= _speed_y * delta_time;
	}

	// Reset falling
	if (Input::GetKeyDown(Input::KeyCode::KEY_P))
	{
		if (_is_falling)
		{
			_speed_y = 0;
			current_position.y = _original_y;
		}
		_is_falling = !_is_falling;
	}

	if (current_position.y < -20)
	{
		_speed_y = 0;
		current_position = _starting_position;
		_is_falling = false;
		SceneManagement::SwitchScene("Assets/Scenes/menu.scene"); // Change this to game over scene
	}

	// Loading scene
	/*if (Input::GetKeyDown(Input::KeyCode::KEY_0))
	{
		SceneManagement::SwitchScene("Assets/Scenes/first_deliver_scene.scene");
	}*/

	transform->SetGlobalPosition(current_position);

	/* Make the player look the mouse */
	math::Plane plane(math::vec(0.0f, current_position.y, 0.0f), math::vec(0.0f, 1.0f, 0.0f));
	math::LineSegment lineSeg = CameraManagement::GetRaycastLineSegment();

	math::vec intersect = plane.ClosestPoint(lineSeg);
	transform->LookAtTarget(intersect);
	/* Make the player look the mouse */
}