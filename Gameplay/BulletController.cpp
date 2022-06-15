#include "scriptingUtil/gameplaypch.h"
#include "BulletController.h"

#include "Scenes.h"
#include "EnemyController.h"
#include "PlayerController.h"
#include "CrystalExplosion.h"

#include <components/ComponentTransform.h>
#include <components/ComponentAgent.h>
#include <components/ComponentObstacle.h>

#include <modules/ModuleSceneManager.h>

Hachiko::Scripting::BulletController::BulletController(GameObject* game_object)
	: Script(game_object, "BulletController")
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
			//	Disable if lifetime is ober
			DeactivateBullet(i);
			continue;
		}
		if (stats.current_charge < stats.charge_time)
		{
			// Do not launch yet
			stats.current_charge += Time::DeltaTime();
			bullet->GetTransform()->SetLocalScale(float3(stats.GetChargedPercent()));
			continue;
		}

		// Move bullet forward
		UpdateBulletTrajectory(i);
		// Check if it collides with an enemy
		if (CheckCollisions(i))
		{
			DeactivateBullet(i);
			continue;
		}

		// If no collision just lower lifetime
		stats.lifetime -= Time::DeltaTime();
	}	
}

void Hachiko::Scripting::BulletController::UpdateBulletTrajectory(unsigned bullet_idx)
{
	GameObject* bullet = _bullets[bullet_idx];
	BulletStats& stats = _bullet_stats[bullet_idx];
	ComponentTransform* bullet_transform = bullet->GetTransform();
	float3 current_position = bullet_transform->GetGlobalPosition();
	stats.prev_position = current_position;
	float3 new_position = current_position + stats.direction * stats.speed * Time::DeltaTime();
	bullet_transform->SetGlobalPosition(new_position);
}

bool Hachiko::Scripting::BulletController::CheckCollisions(unsigned bullet_idx)
{
	// Process hit (if any) for closest enemy or obstacle

	BulletStats& stats = _bullet_stats[bullet_idx];
	GameObject* bullet = _bullets[bullet_idx];
	LineSegment trajectory = LineSegment(stats.prev_position, bullet->GetTransform()->GetGlobalPosition());

	float closest_enemy_hit = FLT_MAX;
	EnemyController* hit_enemy = ProcessEnemies(bullet, stats.size, trajectory, closest_enemy_hit);
	float closest_obstacle_hit = FLT_MAX;
	CrystalExplosion* hit_obstacle = ProcessObstacles(bullet, stats.size, trajectory, closest_obstacle_hit);
	
	if (closest_enemy_hit < closest_obstacle_hit)
	{
		if (hit_enemy)
		{
			float3 knockback_dir = hit_enemy->GetGameObject()->GetTransform()->GetGlobalPosition() - bullet->GetTransform()->GetGlobalPosition();
			hit_enemy->RegisterHit(stats.damage, knockback_dir);
		}
		return true;
	}

	if (hit_obstacle)
	{
		hit_obstacle->RegisterHit(stats.damage);
		return true;
	}

	return false;
}

void Hachiko::Scripting::BulletController::ActivateBullet(unsigned bullet_idx)
{
	_bullet_stats[bullet_idx].alive = true;
	_bullet_stats[bullet_idx].current_charge = 0.f;
	_bullets[bullet_idx]->SetActive(true);
}

void Hachiko::Scripting::BulletController::DeactivateBullet(unsigned bullet_idx)
{
	_bullet_stats[bullet_idx].alive = false;
	_bullets[bullet_idx]->SetActive(false);
}

void Hachiko::Scripting::BulletController::SetBulletTrajectory(ComponentTransform* emitter_transform, unsigned bullet_idx)
{
	GameObject* bullet = _bullets[bullet_idx];
	BulletStats& stats = _bullet_stats[bullet_idx];
	ComponentTransform* bullet_transform = bullet->GetTransform();
	const float bullet_offset = 1.25f;
	float3 emitter_direction = emitter_transform->GetFront().Normalized();
	float3 emitter_position = emitter_transform->GetGlobalPosition() + emitter_direction * bullet_offset;
	
	stats.prev_position = emitter_position;

	stats.direction = emitter_direction;
	bullet_transform->SetGlobalPosition(emitter_position);
}

Hachiko::Scripting::EnemyController* Hachiko::Scripting::BulletController::ProcessEnemies(GameObject* bullet, float bullet_size, LineSegment& trajectory, float& closest_hit)
{
	EnemyController* hit_target = nullptr;
	
	GameObject* agent_container = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
	if (!agent_container)
	{
		return nullptr;
	}

	float3 bullet_position = bullet->GetTransform()->GetGlobalPosition();


	std::vector<GameObject*> enemies = agent_container->children;
	for (int i = 0; i < enemies.size(); ++i)
	{
		GameObject* enemy = enemies[i];
		float3 enemy_position = enemy->GetTransform()->GetGlobalPosition();
		ComponentAgent* agent = enemy->GetComponent<ComponentAgent>();
		if (!agent)
		{
			continue;
		}
		float enemy_radius = agent->GetRadius();
		Sphere hitbox = Sphere(enemy_position, enemy_radius + bullet_size);

		if (enemy->active && trajectory.Intersects(hitbox))
		{

			//float3 dir = enemies_children[i]->GetTransform()->GetGlobalPosition() - bullet->GetTransform()->GetGlobalPosition();
			EnemyController* enemy_controller = enemy->GetComponent<EnemyController>();
			float hit_distance = bullet_position.Distance(enemy_position);
			if (enemy_controller && hit_distance < closest_hit)
			{
				
				closest_hit = hit_distance;
				hit_target = enemy_controller;
			}
		}
	}
	return hit_target;
}

Hachiko::Scripting::CrystalExplosion* Hachiko::Scripting::BulletController::ProcessObstacles(GameObject* bullet, float bullet_size, LineSegment& trajectory, float& closest_hit)
{
	CrystalExplosion* hit_target = nullptr;

	GameObject* obstacle_container = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Crystals");
	if (!obstacle_container)
	{
		return nullptr;
	}

	float3 bullet_position = bullet->GetTransform()->GetGlobalPosition();


	std::vector<GameObject*> obstacles = obstacle_container->children;
	for (int i = 0; i < obstacles.size(); ++i)
	{
		GameObject* obstacle = obstacles[i];
		float3 obstacle_position = obstacle->GetTransform()->GetGlobalPosition();
		// Cylinder obstacles ignore z
		float obstacle_radius = obstacle->GetComponent<ComponentObstacle>()->GetSize().x;
		Sphere hitbox = Sphere(obstacle_position, obstacle_radius + bullet_size);

		if (obstacle->active && trajectory.Intersects(hitbox))
		{
			//float3 dir = enemies_children[i]->GetTransform()->GetGlobalPosition() - bullet->GetTransform()->GetGlobalPosition();
			CrystalExplosion* obstacle_controller = obstacle->GetComponent<CrystalExplosion>();
			float hit_distance = bullet_position.Distance(obstacle_position);
			if (obstacle_controller && hit_distance < closest_hit)
			{
				closest_hit = hit_distance;
				hit_target = obstacle_controller;
			}
		}
	}
	return hit_target;
}

void Hachiko::Scripting::BulletController::SetDamage(int new_damage)
{
	_damage = new_damage;
}

void Hachiko::Scripting::BulletController::SetForward(float3 new_forward)
{
	_direction = new_forward;
}

unsigned* Hachiko::Scripting::BulletController::ShootBullet(ComponentTransform* emitter_transform, BulletStats new_stats)
{
	// We use a pointer to represent when there is no bullet shoot
	for (unsigned i = 0; i < _bullets.size(); i++)
	{
		BulletStats& stats = _bullet_stats[i];
		if (stats.alive)
		{
			// Bullet already being used
			continue;
		}
		// Update stats with the new bullet stats
		stats = new_stats;
		SetBulletTrajectory(emitter_transform, i);

		ActivateBullet(i);
		// After a bullet is generated exit
		return new unsigned(i);
	}
	return nullptr;
}

void Hachiko::Scripting::BulletController::StopBullet(unsigned bullet_index)
{
	DeactivateBullet(bullet_index);
}

const Hachiko::Scripting::BulletController::BulletStats& Hachiko::Scripting::BulletController::GetBulletStats(unsigned bullet_idx) const
{
	// TODO: insert return statement here
	return _bullet_stats[bullet_idx];
}

float Hachiko::Scripting::BulletController::BulletStats::GetChargedPercent()
{
	return current_charge /charge_time;
}
