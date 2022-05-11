#include "scriptingUtil/gameplaypch.h"
#include "PlayerSoundManager.h"
#include "PlayerController.h"
#include "Sounds.h"

Hachiko::Scripting::PlayerSoundManager::PlayerSoundManager(Hachiko::GameObject* game_object)
	: Script(game_object, "PlayerSoundManager")
	, _player_controller(nullptr)
	, _step_timer(0.0f)
	, _step_frequency(0.0f)
{
}

void Hachiko::Scripting::PlayerSoundManager::OnAwake()
{
	_player_controller = game_object->GetComponent<PlayerController>();
}

void Hachiko::Scripting::PlayerSoundManager::OnUpdate()
{
	PlayerState state = _player_controller->GetState();
	float delta_time = Time::DeltaTime();

	switch (state)
	{
	case PlayerState::IDLE:
		_step_timer = 0.0f;
		break;
	case PlayerState::WALKING:
		_step_timer += delta_time;

		if (_step_timer >= _step_frequency)
		{
			_step_timer = 0.0f;
			Audio::Play(Sounds::FOOTSTEP);
		}

		break;
	case PlayerState::MELEE_ATTACKING:
		_step_timer = 0.0f;
		break;
	case PlayerState::RANGED_ATTACKING:
		_step_timer = 0.0f;
		break;
	case PlayerState::DASHING:
		_step_timer = 0.0f;
		break;
	case PlayerState::INVALID:
	default:
		_step_timer = 0.0f;
		break;
	}
}