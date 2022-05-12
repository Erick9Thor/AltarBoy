
#include "scriptingUtil/gameplaypch.h"

#include "EnemyController.h"
#include <PlayerController.h>
#include <Stats.h>
#include <components/ComponentTransform.h>
#include <components/ComponentAgent.h>

Hachiko::Scripting::EnemyController::EnemyController(GameObject* game_object)
	: Script(game_object, "EnemyController")
	, _aggro_range(4)
	, _attack_range(0.5)
	, _spawn_pos(0.0f, 0.0f, 0.0f)
	, _stats()
{
}

void Hachiko::Scripting::EnemyController::OnAwake()
{
	
}

void Hachiko::Scripting::EnemyController::OnStart()
{
	_player = game_object->parent->GetFirstChildWithName("PlayerC");
	_player_controller = _player->GetComponent<PlayerController>();
	transform = game_object->GetComponent<ComponentTransform>();
}

void Hachiko::Scripting::EnemyController::OnUpdate()
{
	if (!_stats.IsAlive())	return;

	_player_pos = _player->GetComponent<ComponentTransform>()->GetGlobalPosition();
	_current_pos = transform->GetGlobalPosition();

	float dist_to_player = (_player_pos - game_object->GetComponent<ComponentTransform>()->GetGlobalPosition()).Length();
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

	if (!_stats.IsAlive())
	{
		DestroyEntity();
	}
	
}

void Hachiko::Scripting::EnemyController::Attack()
{
	_player_controller->_stats.RecieveDamage(_stats._attack_power);
}

void Hachiko::Scripting::EnemyController::ChasePlayer()
{
	_target_pos = Navigation::GetCorrectedPosition(_player_pos, math::float3(10.0f, 10.0f, 10.0f));
	MoveInNavmesh();
}

void Hachiko::Scripting::EnemyController::GoBack()
{
	_target_pos = Navigation::GetCorrectedPosition(_spawn_pos, math::float3(10.0f, 10.0f, 10.0f));
	MoveInNavmesh();
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
	// Check if someone has reference to this object prior to destruction if possible
	//game_object->SetActive(false);
	//SceneManagement::Destroy(game_object);
}
