#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"
#include "entities/player/PlayerState.h"
#include "entities/player/CombatManager.h"


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

	enum class WeaponUsed
	{
		RED = 0,
		GREEN,
		BLUE,
		SIZE
	};

	struct PlayerAttack
	{
		float hit_delay = 0.f;
		float duration = 0.f;
		float dash_distance = 0.f;
		float cooldown = 0.2f;
		CombatManager::AttackStats stats;
	};

	struct Weapon
	{
		std::string name = "Undefined Weapon";
		CombatManager::BulletStats bullet;
		// Here we define the combo of attacks		
		float4 color = float4::zero;
		GameObject* weapon_go = nullptr;
		std::vector<PlayerAttack> attacks;
	};

public:
	PlayerController(GameObject* game_object);
	~PlayerController() override = default;

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate() override;

	PlayerState GetState() const;

	void CheckGoal(const float3& current_position);
	void RegisterHit(float damage_received, bool is_heavy = false, math::float3 direction = float3::zero);
	void UpdateHealthBar();
	void ToggleGodMode();

	bool IsAlive() { return _combat_stats->_current_hp > 0; }
	bool _isInDebug = false;

private:
	math::float3 GetRaycastPosition(
		const math::float3& current_position) const;

	float3 GetCorrectedPosition(const float3& target_pos) const;

	void SpawnGameObject() const;

	// Status check
	bool IsAttacking() const;
	bool IsDashing() const;
	bool IsWalking() const;
	bool IsStunned() const;
	bool IsFalling() const;
	bool IsActionLocked() const;
	bool IsAttackOnCooldown() const;
	bool IsInComboWindow() const;

	const Weapon& GetCurrentWeapon() const;
	const PlayerAttack& GetNextAttack();
	const PlayerAttack& GetCurrentAttack() const;

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
	void ResetPlayer();

public:
	SERIALIZE_FIELD(PlayerState, _state);
	SERIALIZE_FIELD(PlayerState, _previous_state);
	float3 _initial_pos = float3::zero;

	Stats* _combat_stats;
	bool _god_mode = false;
	bool _god_mode_trigger = false;

private:
	SERIALIZE_FIELD(GameObject*, _attack_indicator);
	SERIALIZE_FIELD(GameObject*, _bullet_emitter);
	SERIALIZE_FIELD(GameObject*, _goal);
	SERIALIZE_FIELD(float, _dash_duration);
	SERIALIZE_FIELD(float, _dash_distance);
	SERIALIZE_FIELD(float, _dash_cooldown);
	SERIALIZE_FIELD(int, _max_dash_charges);

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

	std::vector<Weapon> weapons{};

	// Internal state variables
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
	float _current_dash_duration = 0.f;
	float _attack_current_duration = 0.0f;
	float _current_attack_cooldown = 0.f;
	float _attack_current_delay = 0.0f;
	float _rotation_progress = 0.0f;
	float _stun_time = 0.0f;
	float _stun_duration = 0.5f;
	float _falling_distance = 10.0f;
	bool _should_rotate = false;
	bool _is_falling = false;

	const float _ranged_attack_cooldown = 0.2f;
	const float _combo_grace_period = 0.25f;
	const float _attack_forward_movement = 0.8f;
	float _after_attack_timer;
	
	int _current_bullet = -1;

	unsigned _attack_idx = 0;
	unsigned _current_weapon = 0;
	
	GameObject* enemies;
	GameObject* dynamic_envi;

};
} // namespace Scripting
} // namespace Hachiko
