#pragma once

#include <scripting/Script.h>
#include "Stats.h"
#include "PlayerState.h"

namespace Hachiko
{ 
class GameObject;

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

private:
	math::float3 GetRaycastPosition(
		const math::float3& current_position) const;
	
	void MoveDashIndicator(const math::float3& current_position) const;
	
	void SpawnGameObject() const;
	
	void Attack(ComponentTransform* transform, 
		const math::float3& current_position);

	void MeleeAttack(ComponentTransform* transform,
		const math::float3& current_position);

	void RangedAttack(ComponentTransform* transform,
		const math::float3& current_position);

	void Dash(math::float3& current_position);

	void Rotate(ComponentTransform* transform, 
		const math::float3& current_position);
	
	void HandleInput(math::float3& current_position);

public:
	void CheckGoal(const float3& current_position);
	void RegisterEnemyHit(int enemy_atk);
	void UpdateHealthBar();
	
	bool IsAlive() { return _combat_stats._current_hp > 0; }
private:
	SERIALIZE_FIELD(Stats, _combat_stats);

	// Player specific
	SERIALIZE_FIELD(GameObject*, _dash_indicator);
	SERIALIZE_FIELD(GameObject*, _goal);
	SERIALIZE_FIELD(float, _dash_duration);
	SERIALIZE_FIELD(float, _dash_distance);
	SERIALIZE_FIELD(float, _dash_progress);
	SERIALIZE_FIELD(float, _dash_cooldown);
	SERIALIZE_FIELD(float, _dash_timer);
	SERIALIZE_FIELD(int, _dash_count);
	SERIALIZE_FIELD(int, _max_dash_count);
	SERIALIZE_FIELD(bool, _is_dashing);
	SERIALIZE_FIELD(bool, _has_cooldown);
	SERIALIZE_FIELD(bool, _is_falling);
	SERIALIZE_FIELD(math::float3, _dash_start);
	SERIALIZE_FIELD(math::float3, _dash_direction);

	SERIALIZE_FIELD(float, _raycast_min_range);
	SERIALIZE_FIELD(float, _raycast_max_range);
	SERIALIZE_FIELD(float, _attack_radius);
	SERIALIZE_FIELD(float, _attack_cooldown);

	SERIALIZE_FIELD(bool, _should_rotate);
	SERIALIZE_FIELD(float, _rotation_progress);
	SERIALIZE_FIELD(float, _rotation_duration);
	SERIALIZE_FIELD(math::Quat, _rotation_start);
	SERIALIZE_FIELD(math::Quat, _rotation_target);


	SERIALIZE_FIELD(GameObject*, _hp_cell_1);
	SERIALIZE_FIELD(GameObject*, _hp_cell_2);
	SERIALIZE_FIELD(GameObject*, _hp_cell_3);
	std::vector<GameObject*> hp_cells;

	SERIALIZE_FIELD(GameObject*, _camera);
	SERIALIZE_FIELD(GameObject*, _ui_damage);


	float attack_current_cd = 0.0f;

	bool _god_mode = false;
public:
	SERIALIZE_FIELD(PlayerState, _state);
};
} // namespace Scripting
} // namespace Hachiko