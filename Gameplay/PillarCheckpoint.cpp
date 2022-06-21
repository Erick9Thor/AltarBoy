#include "scriptingUtil/gameplaypch.h"
#include "PillarCheckpoint.h"
#include "entities/player/PlayerController.h"

Hachiko::Scripting::PillarCheckpoint::PillarCheckpoint(GameObject* game_object)
	: Script(game_object, "PillarCheckpoint")
	, _is_active(false)
	, _activation_range(5.0f)
	, _restart_position(float3::zero)
	, _animation(nullptr)
	, _player(nullptr)
	, _player_pos(float3::zero)
{
}

void Hachiko::Scripting::PillarCheckpoint::OnAwake()
{
	_animation = game_object->GetComponent<ComponentAnimation>();
	_restart_position = game_object->GetComponent<ComponentTransform>()->GetGlobalPosition();
	_animation = game_object->GetComponent<ComponentAnimation>();
}

void Hachiko::Scripting::PillarCheckpoint::OnStart()
{
	if (!_player)
	{
		HE_LOG("Player object not assigned. Checkpoint disabled.");
		return;
	}
}

void Hachiko::Scripting::PillarCheckpoint::OnUpdate()
{
	if (!_player)	return;

	_player_pos = _player->GetComponent<ComponentTransform>()->GetGlobalPosition();

	if (!_is_active && IsPlayerInRange())
	{
		_is_active = true;
		// Save Checkpoint
		_player->GetComponent<PlayerController>()->_initial_pos = _restart_position;
		// Activate animation
		if (!_animation)
		{
			HE_LOG("No animation found. Could not play.");
			return;
		}
		_animation->StartAnimating();
		
	}
}

bool Hachiko::Scripting::PillarCheckpoint::IsPlayerInRange()
{
	float distance_to_player = _player_pos.Distance(game_object->GetComponent<ComponentTransform>()->GetGlobalPosition());
	if (distance_to_player <= _activation_range)
	{
		return true;
	}

	return false;
}

