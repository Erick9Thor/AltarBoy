#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"
#include "entities/player/PlayerState.h"

enum class WeaponUsed
{
	RED = 0,
	GREEN,
	BLUE,
	SIZE
};

struct Weapon
{
	int attack;
	float cooldown;
	float range;
	float knockback;
};

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
	void OnStart() override;
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
	void CancelAttack();

	// Player simulation
	void MovementController();
	void DashController();
	void DashChargesManager();
	void WalkingOrientationController();
	void AttackController();

	void PickupParasite(const math::float3& current_position);
	void RecieveKnockback(const math::float3 direction);

	void CheckState();

public:
	void CheckGoal(const float3& current_position);
	void RegisterHit(float damage_received, bool is_heavy = false, math::float3 direction = float3::zero);
	void UpdateHealthBar();
	void ToggleGodMode();
	
	bool IsAlive() { return _combat_stats->_current_hp > 0; }
	bool _isInDebug = false;

private:
	SERIALIZE_FIELD(GameObject*, _attack_indicator);
	SERIALIZE_FIELD(GameObject*, _bullet_emitter);
	SERIALIZE_FIELD(GameObject*, _goal);
	SERIALIZE_FIELD(float, _dash_duration);
	SERIALIZE_FIELD(float, _dash_distance);
	SERIALIZE_FIELD(float, _dash_cooldown);
	SERIALIZE_FIELD(int, _max_dash_charges);

	SERIALIZE_FIELD(float, _attack_duration);
	SERIALIZE_FIELD(float, _attack_duration_distance);

	SERIALIZE_FIELD(float, _rotation_duration);

	SERIALIZE_FIELD(GameObject*, _hp_cell_1);
	SERIALIZE_FIELD(GameObject*, _hp_cell_2);
	SERIALIZE_FIELD(GameObject*, _hp_cell_3);
	SERIALIZE_FIELD(GameObject*, _hp_cell_4);
	std::vector<GameObject*> hp_cells;

	SERIALIZE_FIELD(GameObject*, _camera);
	SERIALIZE_FIELD(GameObject*, _ui_damage);

	ComponentTransform* _player_transform = nullptr;
	ComponentAnimation* animation;

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
	WeaponUsed weapon = WeaponUsed::RED;
	bool _should_rotate = false;
	bool _is_falling = false;

	std::vector<Weapon> weapons =
	{
		Weapon{1, 0.33f, 3.5f, 0.5f},
		Weapon{2, 1.0f, 3.5f, 0.7f},
		Weapon{1, 0.6f, 3.5f, 2.0f}
	};
	int _current_bullet = -1;
	

	GameObject* enemies;
	GameObject* dynamic_envi;

public:
	SERIALIZE_FIELD(PlayerState, _state);
	SERIALIZE_FIELD(PlayerState, _previous_state);

	Stats* _combat_stats;
	bool _god_mode = false;
	bool _god_mode_trigger = false;
};
} // namespace Scripting
} // namespace Hachiko
