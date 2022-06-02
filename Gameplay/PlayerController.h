#pragma once

#include <scripting/Script.h>
#include "Stats.h"
#include "PlayerState.h"

namespace Hachiko
{ 
class GameObject;
class ComponentMeshRenderer;
namespace Scripting
{
class PlayerCamera;
class PlayerController : public Script
{
	SERIALIZATION_METHODS(false)

public:
	PlayerController(GameObject* game_object);
	~PlayerController() override = default;

	void OnAwake() override;
	void OnUpdate() override;

	PlayerState GetState() const;
	const Stats& GetStats() { return _combat_stats; }

	math::float3 GetRaycastPosition(
		const math::float3& current_position) const;

private:
	
	void SpawnGameObject() const;
	
	void Attack(ComponentTransform* transform, 
		math::float3& current_position);

	void MeleeAttack(ComponentTransform* transform,
		const math::float3& current_position);

	void RangedAttack(ComponentTransform* transform,
		const math::float3& current_position);

	void Dash(math::float3& current_position);

	void Rotate(ComponentTransform* transform,
		const math::float3& moving_input_dir);
	
	void HandleInput(math::float3& current_position,
		math::float3& moving_input_dir);

public:
	void CheckGoal(const float3& current_position);
	void RegisterEnemyHit(int enemy_atk);
	void UpdateHealthBar();
	
	bool IsAlive() { return _combat_stats._current_hp > 0; }
private:
	SERIALIZE_FIELD(Stats, _combat_stats);

	SERIALIZE_FIELD(GameObject*, _attack_indicator);
	SERIALIZE_FIELD(GameObject*, _goal);
	SERIALIZE_FIELD(float, _dash_duration);
	SERIALIZE_FIELD(float, _dash_distance);
	SERIALIZE_FIELD(float, _dash_cooldown);
	SERIALIZE_FIELD(int, _max_dash_count);

	SERIALIZE_FIELD(float, _raycast_min_range);
	SERIALIZE_FIELD(float, _raycast_max_range);
	SERIALIZE_FIELD(float, _attack_radius);
	SERIALIZE_FIELD(float, _attack_cooldown);
	SERIALIZE_FIELD(float, _attack_duration);

	SERIALIZE_FIELD(float, _rotation_duration);


	SERIALIZE_FIELD(GameObject*, _hp_cell_1);
	SERIALIZE_FIELD(GameObject*, _hp_cell_2);
	SERIALIZE_FIELD(GameObject*, _hp_cell_3);
	SERIALIZE_FIELD(GameObject*, _hp_cell_4);
	std::vector<GameObject*> hp_cells;

	SERIALIZE_FIELD(GameObject*, _camera);
	SERIALIZE_FIELD(GameObject*, _ui_damage);

	float3 _dash_start = float3::zero;
	float3 _dash_direction = float3::zero;
	Quat _rotation_start = Quat::identity;
	Quat _rotation_target = Quat::identity;
	float _dash_count = 0.0f;
	float _dash_progress = 0.0f;
	float _dash_timer = 0.0f;
	float _attack_current_cd = 0.0f;
	float _attack_current_duration = 0.0f;
	float _rotation_progress = 0.0f;
	bool _is_dashing = false;
	bool _dash_has_cooldown = false;
	bool _should_rotate = false;
	bool _is_falling = false;
	bool _is_god_mode = false;

	GameObject* enemies;
	GameObject* dynamic_envi;

public:
	SERIALIZE_FIELD(PlayerState, _state);
};
} // namespace Scripting
} // namespace Hachiko