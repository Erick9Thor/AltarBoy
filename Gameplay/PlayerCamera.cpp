#include "scriptingUtil/gameplaypch.h"
#include "PlayerCamera.h"
#include "PlayerState.h"
#include "PlayerController.h"
#include <components/ComponentTransform.h>
#include <core/Scene.h>
#include <thread>
#include "Sounds.h"

Hachiko::Scripting::PlayerCamera::PlayerCamera(GameObject* game_object)
	: Script(game_object, "PlayerCamera")
	, _relative_position_to_player(math::float3::zero)
	, _player(nullptr)
	, _follow_delay(0.0f)
{
}

void Hachiko::Scripting::PlayerCamera::OnAwake()
{
	_relative_position_to_player = math::float3(0.0f, 15.0f, 10.0f);
	// This is until we have saving for scripts. Because of this, this
	// script needs to be a direct child of scene root, and there must
	// be a GameObject of name "PlayerC" which is our player.
	//_player = game_object->parent->GetFirstChildWithName("PlayerC");
	if (_player != nullptr)
	{
		_player_ctrl = _player->GetComponent<PlayerController>();
	}
	_follow_delay = 0.6f;
	_look_ahead = float3::zero;
	// Seed the rand()
	srand(static_cast <unsigned> (time(0)));

	_audio_source = game_object->GetComponent<ComponentAudioSource>();
}

void Hachiko::Scripting::PlayerCamera::OnStart()
{
	_audio_source->PostEvent(Sounds::PLAY_NAVIGATION);
	_audio_source->PostEvent(Sounds::PLAY_WIND);
	_audio_source->PostEvent(Sounds::PLAY_PEBBLE);
}

void Hachiko::Scripting::PlayerCamera::OnUpdate()
{
	if (_player_ctrl == nullptr)
	{
		if (_player != nullptr)
		{
			_player_ctrl = _player->GetComponent<PlayerController>();
		}
		return;
	}
	// TODO: set some camera offset

	const math::float2 mouse_movement_x_z = MoveCameraWithMouse();
	const math::float3 mouse_movement = float3(mouse_movement_x_z.x, 0.0f, mouse_movement_x_z.y);
	ScrollWheelZoom(&_relative_position_to_player);

	float delay = _follow_delay;

	if (_player_ctrl->_state == PlayerState::WALKING)
	{
		const float look_ahead_time = Time::DeltaTime() / 0.8f;
		Clamp<float>(look_ahead_time, 0.0f, 1.0f);
		_look_ahead = math::float3::Lerp(_look_ahead, _player->GetTransform()->GetFront() * 4, look_ahead_time);
		delay *= 0.25f;
	}
	else
	{
		_look_ahead = float3::zero;
	}

	const math::float3 final_position = _player->GetTransform()->GetGlobalPosition()
		+ _relative_position_to_player + _look_ahead + mouse_movement;
	ComponentTransform* transform = game_object->GetTransform();
	math::float3 current_position = transform->GetGlobalPosition();

	const float delayed_time = Time::DeltaTime() / delay;
	Clamp<float>(delayed_time, 0.0f, 1.0f);

	// Lerp to the pre-defined relative position to the player with a delay: 
	current_position = math::float3::Lerp(current_position, final_position,
		delayed_time);

	float3 shake_offset = float3::zero;

	shake_offset = Shake();

	transform->SetGlobalPosition(current_position + shake_offset);

	// Uncomment the following line if you want the camera to turn itself towards
	// curent player position:
	// transform->LookAtTarget(_player->GetTransform()->GetGlobalPosition());
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