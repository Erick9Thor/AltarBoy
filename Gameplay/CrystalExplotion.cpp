#include "scriptingUtil/gameplaypch.h"

#include "Stats.h"

#include "CrystalExplotion.h"
#include "PlayerController.h"
#include "EnemyController.h"

#include "core/GameObject.h"

#include "components/ComponentTransform.h"

#include "modules/ModuleSceneManager.h"

#include "resources/ResourceAnimation.h"

Hachiko::Scripting::CrystalExplotion::CrystalExplotion(GameObject* game_object)
	: Script(game_object, "CrystalExplotion")
	, _stats(10, 0, 0, 5)
	, _player(nullptr)
	, _explotion_radius(10.0f)
	, _explosive_crystal(false)
{
}

void Hachiko::Scripting::CrystalExplotion::OnAwake()
{
	if (_explotion_crystal->IsActive()) 
	{
		_explotion_crystal->SetActive(false);
	}
	enemies = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
}

void Hachiko::Scripting::CrystalExplotion::OnStart()
{
	transform = game_object->GetTransform();
}

void Hachiko::Scripting::CrystalExplotion::OnUpdate()
{
	if (!_stats.IsAlive() && _explotion_crystal->GetComponent<ComponentAnimation>()->GetCurrentAnimation()->GetCurrentState() == ResourceAnimation::State::STOPPED)
	{
		delete game_object;
	}

	if (!_stats.IsAlive())
	{
		return;
	}

	if (_explosive_crystal)
	{
		CheckRadiusExplotion();
	}

	if (_stats._current_hp <= 0)
	{
		DestroyCristall();
		_stats._is_alive = false;
	}
}

void Hachiko::Scripting::CrystalExplotion::CheckRadiusExplotion()
{
	std::vector<GameObject*> check_hit = {};

	if (enemies != nullptr)
	{
		check_hit = enemies->children;
	}

	check_hit.push_back(_player);

	std::vector<GameObject*> elements_hit = {};
	math::float4x4 inv_matrix = transform->GetGlobalMatrix().Transposed();

	for (int i = 0; i < check_hit.size(); ++i)
	{
		if (check_hit[i]->active && _explotion_radius >= transform->GetGlobalPosition().Distance(check_hit[i]->GetTransform()->GetGlobalPosition()))
		{
			// VS2: EXCEPTION ON QUAD.CPP
			math::float4x4 relative_matrix = check_hit[i]->GetTransform()->GetGlobalMatrix() * inv_matrix;
			math::float3 rel_translate, rel_scale;
			math::Quat rel_rotation;
			relative_matrix.Decompose(rel_translate, rel_rotation, rel_scale);
			float dot_product = transform->GetRight().Dot(rel_translate);
			if (dot_product > 0)
			{
				elements_hit.push_back(check_hit[i]);
			}
		}
	}

	for (Hachiko::GameObject* element : elements_hit)
	{
		EnemyController* enemy_controller = element->GetComponent<EnemyController>();
		PlayerController* player_controller = element->GetComponent<PlayerController>();

		float3 relative_dir = element->GetTransform()->GetGlobalPosition() - transform->GetGlobalPosition();

		if (enemy_controller != nullptr)
		{
			enemy_controller->ReceiveDamage(_stats._attack_power, relative_dir.Normalized());
		}

		if (player_controller != nullptr)
		{
			player_controller->_stats.ReceiveDamage(_stats._attack_power);
		}
	}

	if (elements_hit.size() > 0)
	{
		_stats.ReceiveDamage(5);
	}
}

void Hachiko::Scripting::CrystalExplotion::ReceiveDamage(int damage, float3 direction)
{
	_stats.ReceiveDamage(damage);
}

void Hachiko::Scripting::CrystalExplotion::DestroyCristall()
{

	_static_crystal->SetActive(false);
	_explotion_crystal->SetActive(true);

	_explotion_crystal->GetComponent<ComponentAnimation>()->StartAnimating(_crashing_index, false, 200);
	_stats._is_alive = false;
}
