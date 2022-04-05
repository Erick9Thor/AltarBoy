#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include "components/ComponentTransform.h"

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
	, _movement_speed(0.0f)
	, _dash_distance(0.0f)
	, _dash_duration(0.0f)
	, _is_dashing(false)
	, _dash_progress(0.0f)
	, _dash_start(math::float3::zero)
	, _rotation_speed(0.0f)
{}

void Hachiko::Scripting::PlayerController::OnAwake()
{
	_dash_distance = 2.0f;
	_dash_duration = 0.15f;
	_movement_speed = 10.0f;
	_rotation_speed = 2.5f;
	
	HE_LOG("OnAwake %f", _movement_speed);
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

	float rotation_amount = Input::GetMouseDelta().x * _rotation_speed;
	if (rotation_amount != 0.0f)
	{
		math::Quat current_rotation = transform->GetRotation();
		math::Quat delta_rotation = math::Quat::RotateY(rotation_amount);
		transform->SetRotation(current_rotation * delta_rotation);
	}
	
	math::float3 current_position = transform->GetPosition();
	math::float3 current_front = transform->GetFront();
	math::float3 current_right = transform->GetRight();

	float delta_time = Time::DeltaTime();
	float velocity = _movement_speed * delta_time;

	if (!_is_dashing)
	{
		if (Input::GetKey(Input::KeyCode::KEY_W))
		{
			current_position += current_front * velocity;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_S))
		{
			current_position -= current_front * velocity;
		}

		if (Input::GetKey(Input::KeyCode::KEY_D))
		{
			current_position -= current_right * velocity;
		}
		else if (Input::GetKey(Input::KeyCode::KEY_A))
		{
			current_position += current_right * velocity;
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

	transform->SetPosition(current_position);
}