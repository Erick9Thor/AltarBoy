#include "scriptingUtil/gameplaypch.h"
#include "PlayerCamera.h"
#include <components/ComponentTransform.h>

Hachiko::Scripting::PlayerCamera::PlayerCamera(GameObject* game_object)
	: Script(game_object, "PlayerCamera")
	, _relative_position_to_player(math::float3::zero)
	, _player(nullptr)
	, _delay_amount(0.0f)
{
}

void Hachiko::Scripting::PlayerCamera::OnAwake()
{
	_relative_position_to_player = math::float3(0.0f, 19.7f, 20.5f);

	_player = game_object->parent->GetFirstChildWithName("PlayerC");

	_delay_amount = 1.4f;
}

void Hachiko::Scripting::PlayerCamera::OnStart()
{
}

void Hachiko::Scripting::PlayerCamera::OnUpdate()
{
	math::float3 final_position = _player->GetTransform()->GetGlobalPosition()
		+ _relative_position_to_player;
	ComponentTransform* transform = game_object->GetTransform();
	math::float3 current_position = transform->GetGlobalPosition();

	if (!current_position.Equals(final_position))
	{
		float delayed_time = Time::DeltaTime() / _delay_amount;
		Clamp<float>(delayed_time, 0.0f, 1.0f);

		current_position = math::float3::Lerp(current_position, final_position, 
			delayed_time);

		math::float3 relative_position = final_position - current_position;
		
		if (relative_position.z > 5.0f)
		{
			current_position.z = final_position.z - 5.0f;
		}
		if (relative_position.z < -5.0f)
		{
			current_position.z = final_position.z + 5.0f;
		}
		if (relative_position.x > 5.0f)
		{
			current_position.x = final_position.x - 5.0f;
		}
		if (relative_position.x < -5.0f)
		{
			current_position.x = final_position.x + 5.0f;
		}

		transform->SetGlobalPosition(current_position);
	}
}