#include "scriptingUtil/gameplaypch.h"
#include "PlayerCamera.h"
#include <components/ComponentTransform.h>

Hachiko::Scripting::PlayerCamera::PlayerCamera(GameObject* game_object)
	: Script(game_object, "PlayerCamera")
	, _relative_position_to_player(math::float3::zero)
	, _player(nullptr)
	, _follow_delay(0.0f)
{
}

void Hachiko::Scripting::PlayerCamera::OnAwake()
{
	_relative_position_to_player = math::float3(0.0f, 14.0f, 11.0f);

	// This is until we have saving for scripts. Because of this, this
	// script needs to be a direct child of scene root, and there must
	// be a GameObject of name "PlayerC" which is our player.
	_player = game_object->parent->GetFirstChildWithName("PlayerC");

	_follow_delay = 1.1f;
}

void Hachiko::Scripting::PlayerCamera::OnStart()
{
}

void Hachiko::Scripting::PlayerCamera::OnUpdate()
{
	const math::float3 final_position = _player->GetTransform()->GetGlobalPosition()
		+ _relative_position_to_player;
	ComponentTransform* transform = game_object->GetTransform();
	math::float3 current_position = transform->GetGlobalPosition();

	if (!current_position.Equals(final_position))
	{
		const float delayed_time = Time::DeltaTime() / _follow_delay;
		Clamp<float>(delayed_time, 0.0f, 1.0f);

		// Comment the following if you want z follow to be delayed as well:
		current_position.z = final_position.z;

		// Lerp to the pre-defined relative position to the player with a delay: 
		current_position = math::float3::Lerp(current_position, final_position, 
			delayed_time);


		math::float3 relative_position = final_position - current_position;
		
		constexpr float x_clamp = 2.25f;
		constexpr float z_clamp = 2.5f;
		
		 if (relative_position.z > z_clamp)
		 {
		 	current_position.z = final_position.z - z_clamp;
		 }
		 if (relative_position.z < -z_clamp)
		 {
		 	current_position.z = final_position.z + z_clamp;
		 }
		 
		 if (relative_position.x > x_clamp)
		 {
		 	current_position.x = final_position.x - x_clamp;
		 }
		 if (relative_position.x < -x_clamp)
		 {
		 	current_position.x = final_position.x + x_clamp;
		 }

		transform->SetGlobalPosition(current_position);
	}

	// Uncomment the following line if you want the camera to turn itself towards
	// curent player position:
	//transform->LookAtTarget(_player->GetTransform()->GetGlobalPosition());
}