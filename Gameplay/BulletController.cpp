#include "scriptingUtil/gameplaypch.h"
#include "BulletController.h"

#include "Scenes.h"
#include "EnemyController.h"
#include "PlayerController.h"
#include "CrystalExplosion.h"

#include <components/ComponentTransform.h>

#include <modules/ModuleSceneManager.h>

Hachiko::Scripting::BulletController::BulletController(GameObject* game_object)
	: Script(game_object, "BulletController")
	, _collider_radius(2.0f)
{
}

Hachiko::Scripting::BulletController::~BulletController()
{
	for (unsigned i = 0; i < _bullets.size(); i++)
	{
		SceneManagement::Destroy(_bullets[i]);
	}
}

void Hachiko::Scripting::BulletController::OnAwake()
{
	// Spawn bullet (Passing the prefab can be improved)
	UID bullet_uid = 14999767472668584259;
	_bullets = SceneManagement::Instantiate(bullet_uid, game_object, _max_bullets);

	_bullet_stats.reserve(_bullets.size());

	for (unsigned i = 0; i < _bullets.size(); i++)
	{
		GameObject* bullet = _bullets[i];
		bullet->SetActive(false);
		_bullet_stats.push_back(BulletStats());
	}
	//bullet->GetTransform()->SetGlobalPosition(attack_origin_position);
	//bullet->GetComponent<BulletController>()->SetForward(forward);
	//bullet->GetComponent<BulletController>()->SetDamage(_combat_stats->_attack_power);
}

void Hachiko::Scripting::BulletController::OnUpdate()
{
	
	if (!game_object->IsActive())	return;

	for (unsigned i = 0; i < _bullets.size(); i++)
	{
		GameObject* bullet = _bullets[i];
		// Check if its time to destroy
		BulletStats& stats = _bullet_stats[i];
		if (!stats.alive)
		{
			// Already dead bullet
			continue;
		}
		if(stats.lifetime <= 0)
		{
			//	Disable
			stats.alive = false;
			stats.lifetime = 0.f;
			bullet->SetActive(false);
			continue;
		}

		// Move bullet forward
		ComponentTransform* bullet_transform = bullet->GetTransform();
		float3 current_position = bullet_transform->GetGlobalPosition();
		stats.prev_position = current_position;	
		float3 new_position = current_position + stats.direction * stats.speed * Time::DeltaTime();
		bullet_transform->SetGlobalPosition(new_position);
		// Check if it collides with an enemy
		if (CheckCollisions(i))
		{
			// Disable
			stats.alive = false;
			stats.lifetime = 0.f;
			bullet->SetActive(false);
			continue;
		}

		// If no collision just lower lifetime
		stats.lifetime -= Time::DeltaTime();
	}	
}

bool Hachiko::Scripting::BulletController::CheckCollisions(unsigned bullet_idx)
{
	// Enemy collisions
	// TODO This should be a direct reference


	GameObject* enemies = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
	if (!enemies)	return false;

	BulletStats& stats = _bullet_stats[bullet_idx];
	GameObject* bullet = _bullets[bullet_idx];
	LineSegment trajectory = LineSegment(stats.prev_position, bullet->GetTransform()->GetGlobalPosition());

	std::vector<GameObject*> enemies_children = enemies->children;
	ComponentTransform* transform = game_object->GetTransform();
	math::float4x4 inv_matrix = transform->GetGlobalMatrix().Transposed();

	for (int i = 0; i < enemies_children.size(); ++i)
	{
		float3 enemy_position = enemies_children[i]->GetTransform()->GetGlobalPosition();
		Sphere hitbox = Sphere(enemy_position, _collider_radius);
		if (enemies_children[i]->active && trajectory.Intersects(hitbox))
		{
			
			float3 dir = enemies_children[i]->GetTransform()->GetGlobalPosition() - transform->GetGlobalPosition();
			EnemyController* enemy = enemies_children[i]->GetComponent<EnemyController>();
			if (enemy)
			{
				enemy->RegisterHit(_damage, dir);
			}
			return true;
		}
	}

	// Crystal collisions
	// TODO This should be a direct reference
	GameObject* crystals = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Crystals");
	if (!crystals)	return false;

	std::vector<GameObject*> crystal_children = crystals->children;

	for (int i = 0; i < crystal_children.size(); ++i)
	{
		float3 crystal_position = crystal_children[i]->GetTransform()->GetGlobalPosition();
		Sphere hitbox = Sphere(crystal_position, _collider_radius);
		if (crystal_children[i]->active && trajectory.Intersects(hitbox))
		{
			crystal_children[i]->GetComponent<CrystalExplosion>()->RegisterHit(_damage);
			return true;
		}
	}

	return false;
}

void Hachiko::Scripting::BulletController::SetDamage(int new_damage)
{
	_damage = new_damage;
}

void Hachiko::Scripting::BulletController::SetForward(float3 new_forward)
{
	_direction = new_forward;
}

void Hachiko::Scripting::BulletController::ShootBullet(ComponentTransform* emiter_transform)
{
	//bullet->GetTransform()->SetGlobalPosition(attack_origin_position);
	//bullet->GetComponent<BulletController>()->SetForward(forward);
	//bullet->GetComponent<BulletController>()->SetDamage(_combat_stats->_attack_power);
	//_emitter_transform

	for (unsigned i = 0; i < _bullets.size(); i++)
	{
		BulletStats& stats = _bullet_stats[i];
		if (stats.alive)
		{
			// Bullet already being used
			continue;
		}
		// Reset stats and define shoot moment dependant parameters, improve this stat system so player can set them up
		stats = BulletStats();
		GameObject* bullet = _bullets[i];
		ComponentTransform* bullet_transform = bullet->GetTransform();
		stats.alive = true;
		float3 emitter_position = emiter_transform->GetGlobalPosition();
		stats.prev_position = emitter_position;
		
		stats.direction = emiter_transform->GetFront().Normalized(); stats.direction = emiter_transform->GetFront().Normalized();
		bullet_transform->SetGlobalPosition(emitter_position);
		bullet->SetActive(true);
		// After a bullet is generated exit
		return;
	}
}