#include "scriptingUtil/gameplaypch.h"
#include "BulletController.h"

#include "Scenes.h"
#include "EnemyController.h"
#include "PlayerController.h"

#include <components/ComponentTransform.h>

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
	// Get bullet direction from parent
	CalculateDirection();
	// Get Damage from parent stats
	_damage = game_object->parent->GetComponent<PlayerController>()->_stats._attack_power;
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
		RELEASE(game_object);
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
			game_object->SetActive(false);
		}

		_lifetime -= Time::DeltaTime();
	}
}
void Hachiko::Scripting::BulletController::CalculateDirection()
{
	float3 spawn_pos = game_object->parent->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float3 target_pos = game_object->parent->GetComponent<PlayerController>()->GetRaycastPosition(spawn_pos);
	_direction = (target_pos - spawn_pos);
	_direction.Normalize();
}

bool Hachiko::Scripting::BulletController::CheckCollisions()
{
	std::vector<GameObject*> enemies = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies")->children;
	ComponentTransform* transform = game_object->GetTransform();
	math::float4x4 inv_matrix = transform->GetGlobalMatrix().Transposed();

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i]->active && _collider_radius >= transform->GetGlobalPosition().Distance(enemies[i]->GetTransform()->GetGlobalPosition()))
		{
			math::float4x4 relative_matrix = enemies[i]->GetTransform()->GetGlobalMatrix() * inv_matrix;
			math::float3 rel_translate, rel_scale;
			math::Quat rel_rotation;
			relative_matrix.Decompose(rel_translate, rel_rotation, rel_scale);
			float dot_product = transform->GetRight().Dot(rel_translate);
			if (dot_product > 0)
			{
				//	Merge Lifebar branch first
				// enemies[i]->GetComponent<EnemyController>().RegisterPlayerHit(_damage, dir);
				// _camera->GetComponent<PlayerCamera>()->Shake(0.6f, 0.3f); ??
				return true;
			}
		}
	}

	return false;
}
