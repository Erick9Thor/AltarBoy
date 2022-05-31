#include "scriptingUtil/gameplaypch.h"

#include "constants/Scenes.h"
#include "entities/crystals/CrystalExplosion.h"
#include "entities/enemies/EnemyController.h"
#include "entities/player/BulletController.h"
#include "entities/player/PlayerController.h"

// TODO: Delete this include:
#include <modules/ModuleSceneManager.h>

Hachiko::Scripting::BulletController::BulletController(GameObject* game_object)
	: Script(game_object, "BulletController")
	, _move_speed(0.5f)
	, _direction(1.0f, 1.0f, 1.0f)
	, _lifetime(2.0f)
	, _collider_radius(2.0f)
	, _damage(1)
{
}

void Hachiko::Scripting::BulletController::OnAwake()
{
}

void Hachiko::Scripting::BulletController::OnUpdate()
{
	if (!game_object->IsActive())	return;

	math::float3 current_position = game_object->GetComponent<ComponentTransform>()->GetGlobalPosition();
	const math::float3 delta_pos = _direction * _move_speed;

	// Check if its time to destroy
	if (_lifetime <= 0)
	{
		//	Disable
		game_object->SetActive(false);
		SceneManagement::Destroy(game_object);
		return;
	}
	else
	{
		// Move bullet forward
		current_position += delta_pos;
		game_object->GetComponent<ComponentTransform>()->SetGlobalPosition(current_position);
		// Check if it collides with an enemy
		if (CheckCollisions())
		{
			//	If it hits enemy bullets is destroyed
			SceneManagement::Destroy(game_object);
			return;
		}

		_lifetime -= Time::DeltaTime();
	}
}

bool Hachiko::Scripting::BulletController::CheckCollisions()
{
	// Enemy collisions
	GameObject* enemies = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
	if (!enemies)	return false;

	std::vector<GameObject*> enemies_children = enemies->children;
	ComponentTransform* transform = game_object->GetTransform();
	math::float4x4 inv_matrix = transform->GetGlobalMatrix().Transposed();

	for (int i = 0; i < enemies_children.size(); ++i)
	{
		if (enemies_children[i]->active && _collider_radius >= transform->GetGlobalPosition().Distance(enemies_children[i]->GetTransform()->GetGlobalPosition()))
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
	GameObject* crystals = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Crystals");
	if (!crystals)	return false;

	std::vector<GameObject*> crystal_children = crystals->children;

	for (int i = 0; i < crystal_children.size(); ++i)
	{
		if (crystal_children[i]->active && _collider_radius >= transform->GetGlobalPosition().Distance(crystal_children[i]->GetTransform()->GetGlobalPosition()))
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