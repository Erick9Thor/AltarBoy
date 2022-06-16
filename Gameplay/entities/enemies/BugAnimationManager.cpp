#include "scriptingUtil/gameplaypch.h"
#include "entities/enemies/BugAnimationManager.h"
#include "entities/enemies/EnemyController.h"

Hachiko::Scripting::BugAnimationManager::BugAnimationManager(Hachiko::GameObject* game_object)
	: Script(game_object, "BugAnimationManager")
	, _enemy_controller(nullptr)
	, _previous_state(BugState::INVALID)
	, _state_string("")
	, _animator(nullptr)
	, _idle_index(0)
	, _attacking_index(1)
{
}

void Hachiko::Scripting::BugAnimationManager::OnAwake()
{
	_enemy_controller = 
		game_object->GetComponent<EnemyController>();
}

void Hachiko::Scripting::BugAnimationManager::OnStart()
{
}

void Hachiko::Scripting::BugAnimationManager::OnUpdate()
{
	BugState current_state = _enemy_controller->GetState();
	bool state_changed = current_state != _previous_state;

	if (!state_changed)
	{
		return;
	}

	_previous_state = current_state;

	switch (current_state)
	{
	case BugState::IDLE:
		_state_string = "Play Idle Animation.";
		// _animator->SendTrigger("idle");
		break;
	case BugState::ATTACKING:
		_state_string = "Play Melee Attacking Animation.";
		// _animator->SendTrigger("isAttacking");
		break;
	case BugState::DEAD:
		break;
	case BugState::INVALID:
	default:
		break;
	}
}