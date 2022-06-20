#include "scriptingUtil/gameplaypch.h"
#include "entities/player/PlayerCamera.h"
#include "entities/player/PlayerState.h"
#include "entities/player/PlayerController.h"

// TODO: Delete this include:
#include <core/Scene.h>
#include <thread>
#include "constants/Sounds.h"

Hachiko::Scripting::PlayerCamera::PlayerCamera(GameObject* game_object)
	: Script(game_object, "PlayerCamera")
	, _relative_position_to_player(math::float3(0.0f, 19.0f, 13.0f))
	, _objective(nullptr)
	, _follow_delay(0.6f)
{
}

void Hachiko::Scripting::PlayerCamera::OnAwake()
{
	if (_objective != nullptr)
	{
		_current_objective = _objective;
		_player_ctrl = _objective->GetComponent<PlayerController>();
	}
	_look_ahead = float3::zero;
	_relative_pos_aux = _relative_position_to_player;
	// Seed the rand() for the shaking
	srand(static_cast <unsigned> (time(0)));
}

void Hachiko::Scripting::PlayerCamera::OnStart()
{
}

void Hachiko::Scripting::PlayerCamera::OnUpdate()
{
	CheckForObjective();

	RecalculateRelativePos();

	SetLookAhead();

	MoveCamera();
}

/// <summary>
/// Checks if the objective has changed
/// </summary>
void Hachiko::Scripting::PlayerCamera::CheckForObjective()
{
	if (_current_objective != _objective)
	{
		_current_objective = _objective;
		if (_current_objective != nullptr)
		{
			_player_ctrl = _current_objective->GetComponent<PlayerController>();
		}
	}
}

void Hachiko::Scripting::PlayerCamera::SetLookAhead()
{
	if (_player_ctrl && _player_ctrl->_state == PlayerState::WALKING)
	{
		const float look_ahead_time = Time::DeltaTime() / 0.8f;
		Clamp<float>(look_ahead_time, 0.0f, 1.0f);
		_look_ahead = math::float3::Lerp(_look_ahead, _current_objective->GetTransform()->GetFront() * 5, look_ahead_time);
	}
	else
	{
		_look_ahead = float3::zero;
	}
}

void Hachiko::Scripting::PlayerCamera::MoveCamera()
{
	const math::float2 mouse_movement_x_z = MoveCameraWithMouse();
	const math::float3 mouse_movement = float3(mouse_movement_x_z.x, 0.0f, mouse_movement_x_z.y);
	ScrollWheelZoom(&_relative_position_to_player);

	const math::float3 final_position = _current_objective->GetTransform()->GetGlobalPosition()
		+ _relative_position_to_player + _look_ahead + mouse_movement;
	ComponentTransform* transform = game_object->GetTransform();
	math::float3 current_position = transform->GetGlobalPosition();

	const float delayed_time = Time::DeltaTime() / _follow_delay;
	Clamp<float>(delayed_time, 0.0f, 1.0f);

	// Lerp to the pre-defined relative position to the player with a delay: 
	current_position = math::float3::Lerp(current_position, final_position,
		delayed_time);

	float3 shake_offset = float3::zero;

	shake_offset = Shake();

	transform->SetGlobalPosition(current_position + shake_offset);
}

float2 Hachiko::Scripting::PlayerCamera::MoveCameraWithMouse()
{
	float2 added_movement, mouse_pos;
	mouse_pos = Input::GetMouseNormalizedPosition();
	mouse_pos = mouse_pos - float2(0.5f, 0.5f);
	if (mouse_pos.x > -0.25f && mouse_pos.x < 0.25f)
	{
		mouse_pos.x = 0;
	}
	else
	{
		mouse_pos.x += (mouse_pos.x > 0) ? -0.25f : +0.25f;
	}

	if (mouse_pos.y > -0.25f && mouse_pos.y < 0.25f)
	{
		mouse_pos.y = 0;
	}
	else
	{
		mouse_pos.y += (mouse_pos.y > 0) ? -0.25f : +0.25f;
	}
	const float intensity = 3.0f;
	// We multiply by 4 to set it to 0-1 scale
	added_movement = (mouse_pos * 4.0f) * intensity;
	added_movement = added_movement.Clamp(float2(-intensity, -intensity), float2(intensity, intensity));
	return added_movement;
}

void Hachiko::Scripting::PlayerCamera::ScrollWheelZoom(float3* cam_pos)
{
	int scroll_delta = Input::GetScrollWheelDelta();

	if (scroll_delta > 0 && cam_pos->y > 6.0f)
	{
		cam_pos->y = cam_pos->y - 0.5f;
		cam_pos->z = cam_pos->z - 0.3f;
	}
	else if (scroll_delta < 0 && cam_pos->y < 30.0f)
	{
		cam_pos->y = cam_pos->y + 0.5f;
		cam_pos->z = cam_pos->z + 0.3f;
	}
}

void Hachiko::Scripting::PlayerCamera::Shake(float time, float intensity)
{
	shake_time = time;
	shake_elapsed = 0.0f;
	shake_intensity = intensity;
	shake_magnitude = 1.0f;
}

float3 Hachiko::Scripting::PlayerCamera::Shake()
{
	if (shake_elapsed < shake_time)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float x = (r - 0.5f) * shake_magnitude * shake_intensity;
		r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = (r - 0.5f) * shake_magnitude * shake_intensity;

		shake_elapsed += Time::DeltaTime();
		shake_magnitude = (1 - (shake_elapsed / shake_time)) * (1 - (shake_elapsed / shake_time));
		return float3(x, 0, z);
	} 
	else
	{
		shake_time = 0.0f;
		shake_elapsed = 0.0f;
		return float3::zero;
	}
}

/// <summary>
/// Changes Camera relative position to the one defined. 
/// If time is defined it will revert to its previous position once said time has passed
/// </summary>
/// <param name="new_rel_pos"></param>
/// <param name="time"></param>
void Hachiko::Scripting::PlayerCamera::ChangeRelativePosition(math::float3 new_rel_pos, float time)
{
	if (time > 0.0f)
	{
		// By reverting position we prevent overlaping
		RevertRelativePosition();
		_is_temporary_moved = true;
		_position_timer = time;
	}
	_relative_pos_aux = _relative_position_to_player;
	_relative_position_to_player = new_rel_pos;
}

/// <summary>
/// Reverts Camera relative position to its previous position
/// </summary>
void Hachiko::Scripting::PlayerCamera::RevertRelativePosition()
{
	_relative_position_to_player = _relative_pos_aux;
}

/// <summary>
/// Switches between the defined relative position and its previous one
/// </summary>
/// <param name="new_rel_pos"></param>
void Hachiko::Scripting::PlayerCamera::SwitchRelativePosition(math::float3 new_rel_pos)
{
	if (new_rel_pos.Equals(_relative_position_to_player))
	{
		RevertRelativePosition();
	}
	else
	{
		ChangeRelativePosition(new_rel_pos);
	}
}

void Hachiko::Scripting::PlayerCamera::RecalculateRelativePos()
{
	if (_is_temporary_moved)
	{
		if (_position_timer <= 0.0f)
		{
			RevertRelativePosition();
			_is_temporary_moved = false;
		}
		else
		{
			_position_timer -= Time::DeltaTime();
		}
	}
}