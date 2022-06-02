#include "scriptingUtil/gameplaypch.h"

#include "Stats.h"

#include "CrystalExplosion.h"
#include "PlayerController.h"
#include "EnemyController.h"

#include "core/GameObject.h"

#include "components/ComponentTransform.h"

#include "modules/ModuleSceneManager.h"

#include "resources/ResourceAnimation.h"

Hachiko::Scripting::CrystalExplosion::CrystalExplosion(GameObject* game_object)
	: Script(game_object, "CrystalExplosion")
	, _stats(10, 0, 0, 5)
	, _player(nullptr)
	, _explosion_radius(10.0f)
	, _detecting_radius(1.0f)
	, _explosive_crystal(false)
{
}

void Hachiko::Scripting::CrystalExplosion::OnAwake()
{
	if (_explosion_crystal->IsActive()) 
	{
		_explosion_crystal->SetActive(false);
	}
	enemies = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
}

void Hachiko::Scripting::CrystalExplosion::OnStart()
{
	transform = game_object->GetTransform();
}

void Hachiko::Scripting::CrystalExplosion::OnUpdate()
{
	if (!_stats.IsAlive() && _explosion_crystal->GetComponent<ComponentAnimation>()->GetCurrentAnimation()->GetCurrentState() == ResourceAnimation::State::STOPPED)
	{
		delete game_object;
	}

	if (!_stats.IsAlive())
	{
		return;
	}

	if (_explosive_crystal)
	{
		CheckRadiusExplosion();
	}

	if (_stats._current_hp <= 0)
	{
		DestroyCristall();
		_stats._is_alive = false;
	}
}

void Hachiko::Scripting::CrystalExplosion::CheckRadiusExplosion()
{
	if (_detecting_radius >= transform->GetGlobalPosition().Distance(_player->GetTransform()->GetGlobalPosition()))
	{
		ReceiveDamage(999, float3(0.0f, 0.0f, 0.0f));
	}
}

void Hachiko::Scripting::CrystalExplosion::ExplodeCrystal()
{
	std::vector<GameObject*> check_hit = {};

	if (enemies != nullptr)
	{
		check_hit = enemies->children;
	}

	check_hit.push_back(_player);

	std::vector<GameObject*> elements_hit = {};

	for (int i = 0; i < check_hit.size(); ++i)
	{
		if (check_hit[i]->active &&
			_explosion_radius >= transform->GetGlobalPosition().Distance(check_hit[i]->GetTransform()->GetGlobalPosition()))
		{
			elements_hit.push_back(check_hit[i]);
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
}

void Hachiko::Scripting::CrystalExplosion::ReceiveDamage(int damage, float3 direction)
{
	_stats.ReceiveDamage(damage);
	if (_explosive_crystal)
	{
		ExplodeCrystal();
	}
}

void Hachiko::Scripting::CrystalExplosion::DestroyCristall()
{

	_static_crystal->SetActive(false);
	_explosion_crystal->SetActive(true);

	_explosion_crystal->GetComponent<ComponentAnimation>()->StartAnimating(_crashing_index, false, 200);
	_stats._is_alive = false;
}
