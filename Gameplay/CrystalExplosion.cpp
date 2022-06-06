#include "scriptingUtil/gameplaypch.h"

#include "Stats.h"

#include "CrystalExplosion.h"
#include "PlayerController.h"
#include "EnemyController.h"

#include "core/GameObject.h"

#include "components/ComponentTransform.h"
#include "components/ComponentObstacle.h"

#include "modules/ModuleSceneManager.h"

#include "resources/ResourceAnimation.h"

Hachiko::Scripting::CrystalExplosion::CrystalExplosion(GameObject* game_object)
	: Script(game_object, "CrystalExplosion")
	, _stats()
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
	_stats = game_object->GetComponent<Stats>();
}

void Hachiko::Scripting::CrystalExplosion::OnStart()
{
	transform = game_object->GetTransform();
}

void Hachiko::Scripting::CrystalExplosion::OnUpdate()
{
	if (!_stats->IsAlive() && _explosion_crystal->GetComponent<ComponentAnimation>()->GetCurrentAnimation()->GetCurrentState() == ResourceAnimation::State::STOPPED)
	{
		SceneManagement::Destroy(game_object);
		return;
	}

	if (!_stats || !_stats->IsAlive())
	{
		return;
	}

	if (_explosive_crystal)
	{
		CheckRadiusExplosion();
	}
}

void Hachiko::Scripting::CrystalExplosion::CheckRadiusExplosion()
{
	if (_detecting_radius >= transform->GetGlobalPosition().Distance(_player->GetTransform()->GetGlobalPosition()))
	{
		RegisterHit(_stats->_max_hp);
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
		relative_dir.y = 0.0f;

		if (enemy_controller != nullptr)
		{
			enemy_controller->RegisterHit(_stats->_attack_power, relative_dir.Normalized());
		}

		if (player_controller != nullptr)
		{
			player_controller->RegisterHit(_stats->_attack_power, true, relative_dir.Normalized());
		}
	}
}

void Hachiko::Scripting::CrystalExplosion::RegisterHit(int damage)
{
	if (!_stats)	return;

	_stats->ReceiveDamage(damage);
	if (_explosive_crystal)
	{
		ExplodeCrystal();
	}

	if (_stats->_current_hp <= 0)
	{
		DestroyCrystal();
	}
}

void Hachiko::Scripting::CrystalExplosion::DestroyCrystal()
{

	_static_crystal->SetActive(false);
	_explosion_crystal->SetActive(true);
	ComponentObstacle* obstacle = game_object->GetComponent<ComponentObstacle>();
	if (obstacle)
	{
		obstacle->RemoveObstacle();
	}
	ComponentAnimation* exploding_animation = _explosion_crystal->GetComponent<ComponentAnimation>();
	if (exploding_animation)
	{
		exploding_animation->StartAnimating(_crashing_index, false, 200);
	}
}
