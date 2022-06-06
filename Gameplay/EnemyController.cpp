
#include "scriptingUtil/gameplaypch.h"
#include "EnemyController.h"
#include "PlayerController.h"
#include "Stats.h"
#include "Scenes.h"
#include <components/ComponentTransform.h>
#include <components/ComponentAgent.h>
#include <modules/ModuleSceneManager.h>

Hachiko::Scripting::EnemyController::EnemyController(GameObject* game_object)
	: Script(game_object, "EnemyController")
	, _aggro_range(4)
	, _attack_range(1.5f)
	, _spawn_pos(0.0f, 0.0f, 0.0f)
	, _combat_stats()
	, _spawn_is_initial(false)
	, _player(nullptr)
	, _state(BugState::INVALID)
	, _attack_animation_duration(0.0f)
	, _attack_animation_timer(0.0f)
{
}

void Hachiko::Scripting::EnemyController::OnAwake()
{
	game_object->GetComponent<ComponentAgent>()->AddToCrowd();
	_attack_range = 1.5f;
	_combat_stats = game_object->GetComponent<Stats>();
	_combat_stats->_attack_power = 1;
	_combat_stats->_attack_cd = 1;
	_combat_stats->_move_speed = 4;
	_combat_stats->_max_hp = 4;
	_combat_stats->_current_hp = _combat_stats->_max_hp;
	_stun_time = 0.0f;
	_is_stunned = false;

}

void Hachiko::Scripting::EnemyController::OnStart()
{
	if (_player != nullptr)
	{
		_player_controller = _player->GetComponent<PlayerController>();
	}
	_acceleration = game_object->GetComponent<ComponentAgent>()->GetMaxAcceleration();
	_speed = game_object->GetComponent<ComponentAgent>()->GetMaxSpeed();
	transform = game_object->GetTransform();
	if (_spawn_is_initial)
	{
		_spawn_pos = transform->GetGlobalPosition();
	}
}

void Hachiko::Scripting::EnemyController::OnUpdate()
{
	if (!_combat_stats->IsAlive())
	{
		_state = BugState::DEAD;
		return;
	}

	// TODO: Delete these after seminar and write a better version.
	if (_state == BugState::ATTACKING)
	{
		_attack_animation_timer += Time::DeltaTime();
		
		if (_attack_animation_timer >= _attack_animation_duration)
		{
			_attack_animation_timer = 0.0f;
			_state = BugState::IDLE;
		}
	}
	else
	{
		_state = BugState::IDLE;
	}

	_player_pos = _player->GetTransform()->GetGlobalPosition();
	_current_pos = transform->GetGlobalPosition();
	float dist_to_player = _current_pos.Distance(_player_pos);

	if (dist_to_player > 50)
	{
		return;
	}

	if (_is_stunned)
	{
		if (_stun_time > 0.0f)
		{
			_stun_time -= Time::DeltaTime();
			RecieveKnockback();
			return;
		}
		_is_stunned = false;
		ComponentAgent* agc = game_object->GetComponent<ComponentAgent>();
		// We set the variables back to normal
		agc->SetMaxAcceleration(_acceleration);
		agc->SetMaxSpeed(_speed);
	}
}

BugState Hachiko::Scripting::EnemyController::GetState() const
{
	return _state;
}

const Hachiko::Scripting::Stats* Hachiko::Scripting::EnemyController::GetStats()
{
	return _combat_stats;
}

void Hachiko::Scripting::EnemyController::RegisterHit(int damage, float3 direction)
{
	_combat_stats->ReceiveDamage(damage);
	game_object->ChangeColor(float4(255, 255, 255, 255), 0.3f);
	// Knockback
	_is_stunned = true;
	_stun_time = 0.8f; // Once we have weapons stun duration might be moved to each weapon stat
	float knockback_intensity = 0.5f; // same with knock-back intensity
	_knockback_pos = transform->GetGlobalPosition() + (direction * knockback_intensity);

	if (!_combat_stats->IsAlive())
	{
		DestroyEntity();
	}
}

// Needs to be improved. Player should be able to dodge when enemy starts attacking.
void Hachiko::Scripting::EnemyController::Attack()
{
	_attack_cooldown -= Time::DeltaTime();
	_attack_cooldown = _attack_cooldown < 0.0f ? 0.0f : _attack_cooldown;

	if (_attack_cooldown > 0.0f)
	{
		return;
	}

	_state = BugState::ATTACKING;
	_player_controller->RegisterHit(_combat_stats->_attack_power);
	_attack_cooldown = _combat_stats->_attack_cd;
	
}

void Hachiko::Scripting::EnemyController::ChasePlayer()
{
	_target_pos = Navigation::GetCorrectedPosition(_player_pos, math::float3(10.0f, 10.0f, 10.0f));
	transform->LookAtTarget(_target_pos);
	MoveInNavmesh();
}

void Hachiko::Scripting::EnemyController::GoBack()
{
	_target_pos = Navigation::GetCorrectedPosition(_spawn_pos, math::float3(10.0f, 10.0f, 10.0f));
	transform->LookAtTarget(_target_pos);
	MoveInNavmesh();
}

void Hachiko::Scripting::EnemyController::Stop()
{
	float3 temp_pos = transform->GetGlobalPosition();
	_target_pos = Navigation::GetCorrectedPosition(temp_pos, math::float3(1.0f, 1.0f, 1.0f));
	MoveInNavmesh();
}

void Hachiko::Scripting::EnemyController::RecieveKnockback()
{
	ComponentAgent* agc = game_object->GetComponent<ComponentAgent>();
	_target_pos = Navigation::GetCorrectedPosition(_knockback_pos, math::float3(5.0f, 1.0f, 5.0f));
	// We exagerate the movement
	agc->SetMaxAcceleration(50.0f);
	agc->SetMaxSpeed(30.0f);
	agc->SetTargetPosition(_target_pos);
}

void Hachiko::Scripting::EnemyController::Move()
{
	math::float3 dir = (_target_pos - game_object->GetComponent<ComponentTransform>()->GetGlobalPosition()).Normalized();
	math::float3 step = dir * _combat_stats->_move_speed;
	_current_pos += step;

	transform->SetGlobalPosition(_current_pos);
}

void Hachiko::Scripting::EnemyController::MoveInNavmesh()
{
	ComponentAgent* agc = game_object->GetComponent<ComponentAgent>();
	agc->SetTargetPosition(_target_pos);
}

void Hachiko::Scripting::EnemyController::DestroyEntity()
{
	game_object->SetActive(false);
	RELEASE(game_object);
}
