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

private:
	math::float3 GetRaycastPosition(
		const math::float3& current_position) const;
	
	void SpawnGameObject() const;
	
	// Status check
	bool IsAttacking() const;
	bool IsDashing() const;
	bool IsWalking() const;
	bool IsStunned() const;
	bool IsFalling() const;
	bool IsActionLocked() const;

	// Input and status management
	void HandleInputAndStatus();

	// Actions called by handle input
	void Dash();
	void MeleeAttack();
	void RangedAttack();

	// Player simulation
	void MovementController();
	void DashController();
	void DashChargesManager();
	void WalkingOrientationController();
	void AttackController();

	void RecieveKnockback(math::float3 direction);

public:
	SERIALIZE_FIELD(Stats, _stats);
	void ReceiveDamage(float damage_received, bool is_heavy = false, math::float3 direction = float3::zero);
	void CheckGoal(const float3& current_position);

private:
	SERIALIZE_FIELD(float, _movement_speed);

	SERIALIZE_FIELD(GameObject*, _attack_indicator);
	SERIALIZE_FIELD(GameObject*, _goal);
	SERIALIZE_FIELD(float, _dash_duration);
	SERIALIZE_FIELD(float, _dash_distance);
	SERIALIZE_FIELD(float, _dash_cooldown);
	SERIALIZE_FIELD(int, _max_dash_charges);

	SERIALIZE_FIELD(float, _raycast_min_range);
	SERIALIZE_FIELD(float, _raycast_max_range);
	SERIALIZE_FIELD(float, _attack_radius);
	SERIALIZE_FIELD(float, _attack_cooldown);
	SERIALIZE_FIELD(float, _attack_duration);

	SERIALIZE_FIELD(float, _rotation_duration);

	SERIALIZE_FIELD(GameObject*, _camera);

	SERIALIZE_FIELD(GameObject*, _ui_damage);

	ComponentTransform* _player_transform = nullptr;
	float3 _player_position = float3::zero;
	float3 _movement_direction = float3::zero;
	float3 _dash_start = float3::zero;
	float3 _dash_end = float3::zero;
	float3 _dash_direction = float3::zero;
	float3 _knock_start = float3::zero;
	float3 _knock_end = float3::zero;
	Quat _rotation_start = Quat::identity;
	Quat _rotation_target = Quat::identity;
	float _dash_charges = 0.0f;
	float _dash_progress = 0.0f;
	float _dash_charging_time = 0.0f;
	float _attack_current_cd = 0.0f;
	float _attack_current_duration = 0.0f;
	float _rotation_progress = 0.0f;
	float _stun_time = 0.0f;
	float _stun_duration = 0.5f;
	float _falling_distance = 10.0f;
	bool _should_rotate = false;
	bool _is_god_mode = false;

	GameObject* enemies;
	GameObject* dynamic_envi;

public:
	SERIALIZE_FIELD(PlayerState, _state);
};
} // namespace Scripting
} // namespace Hachiko