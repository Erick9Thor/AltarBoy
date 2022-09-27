#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"
#include "entities/player/CombatManager.h"
#include "entities/player/PlayerCamera.h"
#include "misc/GauntletManager.h"
#include "misc/LevelManager.h"

#include <queue>

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;
	class ComponentParticleSystem;
	class ComponentBillboard;
	class ComponentProgressBar;

	namespace Scripting
	{
		class AudioManager;
		class PlayerController;
		class CombatVisualEffectsPool;

		class PlayerCamera;

		enum class BossState
		{
			WAITING_ENCOUNTER,
			STARTING_ENCOUNTER,
			COMBAT_FORM,
			CACOON_FORM,
			DEAD,
		};

		// States while in BossState::COMBAT_FORM
		enum class CombatState
		{
			IDLE,
			CHASING,
			ATTACKING,
			SPAWNING_CRYSTALS,
			CONSUMING_PARASYTES,
			CRYSTAL_JUMP,
		};

		class BossController : public Script
		{
			SERIALIZATION_METHODS(false)

			enum class BossWeaponUsed
			{
				RED = 0,
				GREEN,
				BLUE,
				SIZE
			};

			struct BossAttack
			{
				float hit_delay = 0.f;
				float duration = 0.f;
				float cooldown = 0.2f;
				CombatManager::AttackStats stats;
			};

			enum class AttackType
			{
				MELEE_CLAW = 0,
				SWORD,
				HAMMER,
			};

			struct Weapon
			{
				std::string name = "Undefined Weapon";
				// Here we define the combo of attacks		
				float4 color = float4::zero;
				bool unlimited = true;
				unsigned charges = 0;
				GameObject* weapon_go = nullptr;
				std::vector<BossAttack> attacks;
			};

		public:
			BossController(GameObject* game_object);
			~BossController() override = default;

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

			BossState GetState() const { return state; };
			bool IsAlive() const;
			void RegisterHit(int dmg);
			void UpdateHpBar();


		private:
			
			// WEAPON MANAGE
			void ChangeWeapon(unsigned weapon_idx);
			void CreateBossWeapons();

			Weapon& GetCurrentWeapon();
			const BossAttack& GetNextAttack();
			const BossAttack& GetCurrentAttack();

			void SetUpHpBar();
			void SetHpBarActive(bool v);
			void StateController();
			void StateTransitionController();
			void CombatController();
			void CombatTransitionController();

			float GetPlayerDistance();

			// Fight States
			void WaitingController();
			void StartEncounter();
			void StartEncounterController();
			void ResetCombatState();
			void Die();
			void DieController();

			float4x4 GetMeleeAttackOrigin(float attack_range) const;

			// Cacoon
			void StartCacoon();
			void CacoonController();

			bool CacoonTrigger();
			void FinishCacoon();

			// Combat States (While COMBAT_FORM)
			void Chase();
			void ChaseController();

			void MeleeAttack();
			void MeleeAttackController();

			void SpawnCrystals();
			void SpawnCrystalsController();
			void ConsumeParasytes();
			void ConsumeParasytesController();

			void FocusCamera(bool focus_on_boss);

			BossAttack GetAttackType(AttackType attack_type);

		private:
			SERIALIZE_FIELD(int, state_value);
			SERIALIZE_FIELD(GameObject*, hp_bar_go);
			SERIALIZE_FIELD(GameObject*, crystal_target_go);
			SERIALIZE_FIELD(GameObject*, cocoon_placeholder_go);
			SERIALIZE_FIELD(GameObject*, gauntlet_go);
			SERIALIZE_FIELD(float, start_encounter_range);
			GameObject* player = nullptr; // It's found on scene based on name
			LevelManager* level_manager = nullptr; // It's found on scene based on name
			PlayerCamera* player_camera = nullptr; // It's found on scene based on name
			ComponentTransform* transform = nullptr;
			ComponentProgressBar* hp_bar = nullptr;
			ComponentAgent* agent = nullptr;
			GauntletManager* gauntlet = nullptr;
			CombatManager* combat_manager = nullptr;
			Stats* combat_stats = nullptr;
			BossState state = BossState::WAITING_ENCOUNTER;
			BossState prev_state = state;
			CombatState combat_state = CombatState::IDLE;
			CombatState prev_combat_state = combat_state;
			bool hitable = true;
			std::vector<float> gauntlet_thresholds_percent{ 0.3, 0.7 };
			float3 target_position = float3::zero;

			float attack_current_cd;

			bool camera_focus_on_boss = false;
			float time_elapse = 0.0;

			std::vector<Weapon> weapons{};
			unsigned _current_weapon = 0;
			unsigned _attack_idx = 0;
			unsigned _attack_charges = 0;

			SERIALIZE_FIELD(GameObject*, _sword_weapon);
			SERIALIZE_FIELD(GameObject*, _claw_weapon);
			SERIALIZE_FIELD(GameObject*, _hammer_weapon);
		};
	} // namespace Scripting
} // namespace Hachiko*/
