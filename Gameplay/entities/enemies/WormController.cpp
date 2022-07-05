#include "scriptingUtil/gameplaypch.h"
#include "constants/Scenes.h"
#include "entities/Stats.h"
#include "entities/enemies/WormController.h"
#include "entities/player/PlayerController.h"
#include "entities/player/PlayerCamera.h"
#include "constants/Sounds.h"

//// TODO: Delete this include:
//#include <modules/ModuleSceneManager.h>

#include <resources/ResourceAnimation.h>
#include <components/ComponentAgent.h>
#include <components/ComponentTransform.h>

#include "misc/AudioManager.h"

Hachiko::Scripting::WormController::WormController(GameObject* game_object)
	: Script(game_object, "WormController")
	, _spawning_time(2.0f)
	, _enemy_body(nullptr)
	, _small_dust_particles(nullptr)
	, _big_dust_particles(nullptr)
{

	// Push attack
	push_attack.damage = 0;
	push_attack.knockback_distance = 5.0f;
	push_attack.width = 0.f;
	push_attack.range = 5.0f; // a bit bigger than its attack activation range
	push_attack.type = CombatManager::AttackType::CIRCLE;
}

void Hachiko::Scripting::WormController::OnAwake()
{
	animation = game_object->GetComponent<ComponentAnimation>();
	animation->StartAnimating();
	_combat_manager = Scenes::GetCombatManager()->GetComponent<CombatManager>();
	_player_camera = Scenes::GetMainCamera()->GetComponent<PlayerCamera>();
	if (_small_dust)
	{
		//_small_dust_particles = _small_dust->GetComponent<ComponentParticleSystem>();
	}
	if (_big_dust)
	{
		//_big_dust_particles = _big_dust->GetComponent<ComponentParticleSystem>();
	}
	_has_spawned = false;
}

void Hachiko::Scripting::WormController::OnStart()
{
	transform = game_object->GetTransform();
	if (!_has_spawned)
	{
		Spawn();
	}
}

void Hachiko::Scripting::WormController::OnUpdate()
{
	if (!_has_spawned)
	{
		return;
	}
	CheckState();
	SpawnController();
}

void Hachiko::Scripting::WormController::SpawnController()
{
	if (_enemy_body->IsActive() && _state != WormState::SPAWNING)
	{
		return;
	}

	if (_enemy_body->IsActive() && _state == WormState::SPAWNING && animation->IsAnimationStopped())
	{
		_state = WormState::IDLE;
		return;
	}

	if (_state == WormState::SPAWNING && _enemy_body)
	{
		_player_camera->Shake(0.5f, 0.8f);
		_enemy_body->SetActive(true);
		//_big_dust_particles->Play();
		//Push the player back
		_combat_manager->EnemyMeleeAttack(transform->GetGlobalMatrix(), push_attack);
		return;
	}

	_current_spawning_time -= Time::DeltaTime();
	if (_current_spawning_time <= 0.0f)
	{
		_state = WormState::SPAWNING;
		animation->ResetState();
	}
}

Hachiko::Scripting::WormState Hachiko::Scripting::WormController::GetState() const
{
	return _state;
}

void Hachiko::Scripting::WormController::Spawn()
{
	_has_spawned = true;
	if (_enemy_body)
	{
		_enemy_body->SetActive(false);
	}
	_state = WormState::INVALID;
	//_small_dust_particles->Play();
	_current_spawning_time = _spawning_time;
	_player_camera->Shake(_spawning_time, 0.2f);
}

void Hachiko::Scripting::WormController::CheckState()
{
	WormState current_state = GetState();
	bool state_changed = current_state != _previous_state;

	if (!state_changed)
	{
		return;
	}

	//if ((_previous_state == WormState::ATTACKING || _previous_state == BugState::MOVING) &&
	//	(current_state == BugState::IDLE || current_state == BugState::MOVING_BACK || current_state == BugState::DEAD))
	//{
	//	if (_already_in_combat)
	//	{
	//		_audio_manager->UnregisterCombat();
	//		_already_in_combat = false;
	//	}
	//}
	//else if ((current_state == BugState::ATTACKING || current_state == BugState::MOVING) &&
	//	(_previous_state == BugState::IDLE || _previous_state == BugState::MOVING_BACK || _previous_state == BugState::DEAD))
	//{
	//	if (!_already_in_combat)
	//	{
	//		_audio_manager->RegisterCombat();
	//		_already_in_combat = true;
	//	}
	//}

	_previous_state = current_state;

	switch (current_state)
	{
	case WormState::IDLE:
		animation->SendTrigger("idle");
		break;
	case WormState::SPAWNING:
		//_audio_source->PostEvent(Sounds::ENEMY_ATTACK);
		animation->SendTrigger("isAppear");
		break;
	case WormState::DEAD:
		//_audio_source->PostEvent(Sounds::ENEMY_DIE);
		animation->SendTrigger("isDead");
		break;
	case WormState::HIDEN:
		animation->SendTrigger("isHide");
		break;
	case WormState::ATTACKING:
		animation->SendTrigger("isAttack");
		break;
	case WormState::HIT:
		animation->SendTrigger("isHit");
		break;
	case WormState::INVALID:
	default:
		break;
	}
}