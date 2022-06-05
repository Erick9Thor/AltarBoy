
#include "scriptingUtil/gameplaypch.h"
#include "EnemyController.h"
#include "PlayerController.h"
#include "Stats.h"
#include <components/ComponentTransform.h>
#include <components/ComponentAgent.h>

Hachiko::Scripting::EnemyController::EnemyController(GameObject* game_object)
	: Script(game_object, "EnemyController")
	, _aggro_range(4)
	, _attack_range(1.5f)
	, _spawn_pos(0.0f, 0.0f, 0.0f)
	, _spawn_is_initial(false)
	, _stats(2, 2, 5, 10)
	, _player(nullptr)
{
}

void Hachiko::Scripting::EnemyController::OnAwake()
{
	game_object->GetComponent<ComponentAgent>()->AddToCrowd();
	_attack_range = 1.5f;
	_stun_time = 0.0f;
	_is_stunned = false;
}

void Hachiko::Scripting::EnemyController::OnStart()
{
	// TODO: Find by name in scene.
	_player_controller = _player->GetComponent<PlayerController>();
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
	if (!_stats.IsAlive())
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

	_player_pos = _player->GetTransform()->GetGlobalPosition();
	_current_pos = transform->GetGlobalPosition();

	float dist_to_player = _current_pos.Distance(_player_pos);
	if (dist_to_player <= _aggro_range)
	{
		if (dist_to_player <= _attack_range)
		{
			Attack();
		}
		else
		{
			ChasePlayer();
		}
	}
	else
	{
		GoBack();
	}

	if (_stats._current_hp <= 0)
	{
		DestroyEntity();
		_stats._is_alive = false;
	}
	
}

Hachiko::Scripting::Stats& Hachiko::Scripting::EnemyController::GetStats()
{
	return _stats;
}

void Hachiko::Scripting::EnemyController::ReceiveDamage(int damage, float3 direction)
{
	_stats.ReceiveDamage(damage);
	game_object->ChangeColor(float4(255, 255, 255, 255), 0.2f);
	_is_stunned = true;
	_stun_time = 1.2f; // Once we have weapons stun duration might be moved to each weapon stat
	float knockback_intensity = 0.5f; // same with knock-back intensity
	_knockback_pos = transform->GetGlobalPosition() + (direction * knockback_intensity);
}

void Hachiko::Scripting::EnemyController::Attack()
{
	_attack_cooldown -= Time::DeltaTime();
	_attack_cooldown = _attack_cooldown < 0.0f ? 0.0f : _attack_cooldown;

	if (_attack_cooldown > 0.0f)
	{
		return;
	}

	_player_controller->_stats.ReceiveDamage(_stats._attack_power);
	_attack_cooldown = _stats._attack_cd;
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
	math::float3 step = dir * _stats._move_speed;
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
	delete game_object;
}
