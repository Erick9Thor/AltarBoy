#include "scriptingUtil/gameplaypch.h"
#include "entities/player/PlayerAnimationManager.h"
#include "entities/player/PlayerController.h"

Hachiko::Scripting::PlayerAnimationManager::PlayerAnimationManager(GameObject* game_object)
	: Script(game_object, "PlayerAnimationManager")
	, _player_controller(nullptr)
	, _previous_state(PlayerState::INVALID)
	, _state_string("")
	, _animator(nullptr)
	, _idle_index(0)
	, _walking_index(1)
	, _melee_index(2)
	, _ranged_index(3)
	, _dashing_index(4)
{
}

void Hachiko::Scripting::PlayerAnimationManager::OnAwake()
{
	_player_controller = game_object->GetComponent<PlayerController>();
}

void Hachiko::Scripting::PlayerAnimationManager::OnStart()
{
}

void Hachiko::Scripting::PlayerAnimationManager::OnUpdate()
{
	PlayerState current_state = _player_controller->GetState();
	bool state_changed = current_state != _previous_state;
	
	if (!state_changed)
	{
		return;
	}

	_previous_state = current_state;

	switch (current_state)
	{
	case PlayerState::IDLE:
		_state_string = "Play Idle Animation.";
		_animator->SendTrigger("isIdle");
		break;
	case PlayerState::WALKING:
		_state_string = "Play Walking Animation.";
		_animator->SendTrigger("isRuning");
		break;
	case PlayerState::MELEE_ATTACKING:
		_state_string = "Play Melee Attacking Animation.";
		_animator->SendTrigger("isMeleeAtack");
		break;
	case PlayerState::RANGED_ATTACKING:
		_state_string = "Play Ranged Attacking Animation.";
		_animator->SendTrigger("isRangedAttack");
		break;
	case PlayerState::DASHING:
		_state_string = "Play Dashing Animation.";
		_animator->SendTrigger("isDashing");
		break;
	case PlayerState::FALLING:
		_state_string = "Play Falling Animation.";
		_animator->SendTrigger("isFalling");
		break;
	
	case PlayerState::INVALID:
	default:
		break;
	}
}