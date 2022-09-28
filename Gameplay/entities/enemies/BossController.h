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
            JUMPING,
        };

        // States while the boss is in CombatState::JUMPING
        enum class JumpingState
        {
            NOT_TRIGGERED,
            WAITING_TO_JUMP,
            ASCENDING,
            POST_ASCENDING_DELAY,
            ON_HIGHEST_POINT,
            POST_ON_HIGHEST_POINT,
            DESCENDING,
            POST_DESCENDING_DELAY,
            GETTING_UP,
            WAITING_FOR_SKILL,
            CASTING_SKILL,
            POST_CAST_SKILL,
            COUNT
        };

        // The follow up action for jump:
        enum class JumpingMode
        {
            BASIC_ATTACK,
            WEAPON_CHANGE,
            STALAGMITE,
            CRYSTAL,
            DETERMINED_BY_PATTERN
        };

        namespace JumpUtil
        {
            constexpr int SPECIAL_JUMP_PATTERN_SIZE = 5;
        }

        class BossController : public Script
        {
            SERIALIZATION_METHODS(false)

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

            void TriggerJumpingState(JumpingMode jumping_mode);
            void ResetJumpingState();
            void ExecuteJumpingState();
            void UpdateAscendingPosition() const;
            void UpdateDescendingPosition() const;
            void DetermineSpecialJumpingMode();
            JumpingMode GetSpecialJumpingMode() const;

            void ChangeStateText(const char* state_string) const;

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
            std::vector<float> gauntlet_thresholds_percent{0.3f, 0.7f};
            float3 target_position = float3::zero;

            // Jumping state related fields:
            SERIALIZE_FIELD(float, _jump_start_delay);
            SERIALIZE_FIELD(float, _jump_ascending_duration);
            SERIALIZE_FIELD(float, _jump_post_ascending_delay);
            SERIALIZE_FIELD(float, _jump_on_highest_point_duration);
            SERIALIZE_FIELD(float, _jump_post_on_highest_point_delay);
            SERIALIZE_FIELD(float, _jump_descending_duration);
            SERIALIZE_FIELD(float, _jump_post_descending_delay);
            SERIALIZE_FIELD(float, _jump_getting_up_duration);
            SERIALIZE_FIELD(float, _jump_skill_delay);
            SERIALIZE_FIELD(float, _jump_skill_duration);
            SERIALIZE_FIELD(float, _jump_post_on_land_aoe_duration);
            // Height which the boss reaches highest during the jump:
            SERIALIZE_FIELD(float, _jump_height);
            // The offset relative to the player position where the boss should
            // land:
            SERIALIZE_FIELD(float, _jump_offset);
            JumpingState _jumping_state;
            float3 _jump_start_position;
            float3 _jump_mid_position;
            float3 _jump_end_position;
            SERIALIZE_FIELD(float3, _crystal_jump_position);
            // Timer that is used to interpolate to the next state of the
            // jumping action carried out by boss. Final value is dependent on
            // _jumping_state and the corresponding delay/duration.
            float _jumping_timer;
            // Jump pattern when the Jumping mode was set to
            // JumpingMode::DETERMINED_BY_PATTERN:
            int _jump_pattern_index;
            JumpingMode _special_jump_pattern[
                JumpUtil::SPECIAL_JUMP_PATTERN_SIZE] = {
                JumpingMode::STALAGMITE,
                JumpingMode::WEAPON_CHANGE,
                JumpingMode::WEAPON_CHANGE,
                JumpingMode::STALAGMITE,
                JumpingMode::WEAPON_CHANGE
            };
            JumpingMode _current_jumping_mode;

            // Debug variables:
            SERIALIZE_FIELD(ComponentText*, _boss_state_text_ui);

            float attack_current_cd;

            bool camera_focus_on_boss = false;
            float time_elapse = 0.0;
        };
    } // namespace Scripting
} // namespace Hachiko
