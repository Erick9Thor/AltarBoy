#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include "Scenes.h"

#include <components/ComponentTransform.h>
#include <components/ComponentCamera.h>
#include <modules/ModuleSceneManager.h>

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
	, _movement_speed(0.0f)
	, _dash_distance(0.0f)
	, _dash_duration(0.0f)
	, _is_dashing(false)
	, _dash_progress(0.0f)
	, _dash_start(math::float3::zero)
	, _is_falling(false)
	, _original_y(0.0f)
	, _speed_y(0.0f)
	, _starting_position(math::float3::zero)
	, _rotation_progress(0.0f)
	, _rotation_target(math::Quat::identity)
	, _should_rotate(false)
{
}

void Hachiko::Scripting::PlayerController::OnAwake()
{
	_dash_distance = 2.0f;
	_dash_duration = 0.15f;
	_movement_speed = 5.0f;
	_rotation_duration = 0.075f;

	_original_y = game_object->GetTransform()->GetGlobalPosition().y;
	_speed_y = 0.0f;
	
	_starting_position = math::float3(0.0f, 1.0f, 0.0f);
}

void Hachiko::Scripting::PlayerController::OnStart()
{
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

	float delta_time = Time::DeltaTime();
	float velocity = _movement_speed * delta_time;

	GameObject* raycast_hit = SceneManagement::Raycast(
		current_position - math::float3(0.0f, 1.0f, 0.0f), current_position);


	if (raycast_hit == nullptr || raycast_hit->name == "Player") 
	{
		_is_falling = true;
	}

	if (raycast_hit != nullptr && raycast_hit->name == "Goal")
	{
		// TODO: Uncomment this in the next PR after adding the new scenes with
		// YAML based serialization.
		// 
		// SceneManagement::SwitchScene(Scenes::WIN);
	}

	// TODO: Delete this in the next PR after adding the new scenes with
	// YAML based serialization. For now, player is not falling by default.
	_is_falling = false;


	if (!_is_dashing && !_is_falling)
	{
		math::float3 delta_x = math::float3::unitX * velocity;
		math::float3 delta_y = math::float3::unitY * velocity;
		math::float3 delta_z = math::float3::unitZ * velocity;

		if (Input::GetKey(Input::KeyCode::KEY_W))
		{
			current_position -= delta_z;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_S))
		{
			current_position += delta_z;
		}

		if (Input::GetKey(Input::KeyCode::KEY_D))
		{
			current_position += delta_x;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_A))
		{
			current_position -= delta_x;
		}

		if (Input::GetKey(Input::KeyCode::KEY_Q))
		{
			current_position += delta_y;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_E))
		{
			current_position -= delta_y;
		}

		if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
		{
			_is_dashing = true;
			_dash_progress = 0.0f;
			_dash_start = current_position;
		}
	}

	//if (_is_falling)
	//{
	//	_speed_y += 25.0f * delta_time;
	//	current_position.y -= _speed_y * delta_time;
	//}

	//// Reset falling:
	//if (Input::GetKeyDown(Input::KeyCode::KEY_P))
	//{
	//	if (_is_falling)
	//	{
	//		_speed_y = 0;
	//		current_position.y = _original_y;
	//	}
	//	_is_falling = !_is_falling;
	//}

	static int times_hit_g = 0;
	if (Input::GetKeyDown(Input::KeyCode::KEY_G))
	{
		std::string name = "GameObject ";
		
		name += std::to_string(times_hit_g);
		
		times_hit_g++;

		GameObject* created_game_object = GameObject::Instantiate();

		created_game_object->SetName(name);
	}

	// TODO: Uncomment this in the next PR after adding the new scenes with
	// YAML based serialization.
	/*if (current_position.y < -20)
	{
		_speed_y = 0;
		current_position = _starting_position;
		_is_falling = false;

		SceneManagement::SwitchScene(Scenes::LOSE);
	}*/

	// If the player position is changed, check if rotation is needed:
	if (!current_position.Equals(transform->GetGlobalPosition()))
	{
		// Get the position player should be looking at:
		math::float3 look_at_position = current_position;
		look_at_position.y = transform->GetGlobalPosition().y;

		// Calculate the direction player should be looking at:
		math::float3 look_at_direction = (transform->GetGlobalPosition() - look_at_position);
		look_at_direction.Normalize();

		// Get the rotation player is going to have:
		math::Quat target_rotation = transform->SimulateLookAt(look_at_direction);

		// If rotation is gonna be changed fire up the rotating process:
		if (!target_rotation.Equals(transform->GetGlobalRotation()))
		{
			// Reset all the variables related to rotation lerp:
			_rotation_progress = 0.0f;
			_rotation_target = target_rotation;
			_rotation_start = transform->GetGlobalRotation();
			_should_rotate = true;
		}
	}

	if (_should_rotate)
	{
		_rotation_progress += delta_time / _rotation_duration;
		_rotation_progress = _rotation_progress > 1.0f ? 1.0f : _rotation_progress;

		transform->SetGlobalRotation(
			Quat::Lerp(_rotation_start, _rotation_target, _rotation_progress));

		_should_rotate = _rotation_progress != 1.0f;

		if (!_should_rotate)
		{
			_rotation_progress = 0.0f;
			_rotation_target = math::Quat::identity;
			_rotation_start = math::Quat::identity;
			_should_rotate = false;
		}
	}

	if (_is_dashing)
	{
		_dash_progress += delta_time / _dash_duration;
		_dash_progress = _dash_progress > 1.0f ? 1.0f : _dash_progress;

		_is_dashing = (_dash_progress < 1.0f);

		math::float3 _dash_end = _dash_start + transform->GetFront() * _dash_distance;

		current_position = math::float3::Lerp(_dash_start, _dash_end, _dash_progress);
	}

	if (!_is_dashing && Input::GetMouseButton(Input::MouseButton::LEFT))
	{
		math::Plane plane(
			math::float3(0.0f, current_position.y, 0.0f),
			math::float3(0.0f, 1.0f, 0.0f));

		math::float2 mouse_position_view =
			ComponentCamera::ScreenPositionToView(Input::GetMousePosition());

		math::LineSegment ray = Debug::GetRenderingCamera()->Raycast(
			mouse_position_view.x, mouse_position_view.y);

		math::float3 intersection_position = plane.ClosestPoint(ray);

		// Make the player look the mouse:
		transform->LookAtTarget(intersection_position);
	}

	// Apply the position:
	transform->SetGlobalPosition(current_position);
}