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
	, _rotation_speed(0.0f)
	, _is_falling(false)
	, _original_y(0.0f)
	, _speed_y(0.0f)
	, _starting_position(math::float3::zero)
{
}

void Hachiko::Scripting::PlayerController::OnAwake()
{
	_dash_distance = 4.0f;
	_dash_duration = 0.15f;
	_movement_speed = 5.0f;
	_rotation_speed = 4.0f;

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
	math::Quat current_rotation = transform->GetGlobalRotation();
	math::float3 current_front = transform->GetFront();
	math::float3 current_right = transform->GetRight();

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
		math::float3 delta_z = transform->GetFront().Normalized() * velocity;
		math::float3 delta_x = transform->GetRight().Normalized() * velocity;
		math::float3 delta_y = math::float3::unitY * velocity;
		
		float delta_rotation_y = delta_time * _rotation_speed;

		if (Input::GetKey(Input::KeyCode::KEY_W))
		{
			current_position += delta_z;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_S))
		{
			current_position -= delta_z;
		}

		if (Input::GetKey(Input::KeyCode::KEY_D))
		{
			current_rotation = 
				current_rotation * Quat::RotateY(-delta_rotation_y);
		}
		else if (Input::GetKey(Input::KeyCode::KEY_A))
		{
			current_rotation =
				current_rotation * Quat::RotateY(delta_rotation_y);
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

	if (_is_dashing)
	{
		_dash_progress += delta_time / _dash_duration;
		_dash_progress = _dash_progress > 1.0f ? 1.0f : _dash_progress;

		_is_dashing = (_dash_progress < 1.0f);

		math::float3 _dash_end = _dash_start + current_front * _dash_distance;

		current_position = math::float3::Lerp(_dash_start, _dash_end, _dash_progress);
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

	// Apply the position:
	transform->SetGlobalPosition(current_position);
	// Apply the rotation:
	transform->SetGlobalRotation(current_rotation);

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

}