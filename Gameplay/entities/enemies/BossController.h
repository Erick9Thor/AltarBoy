#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"
#include "entities/player/CombatManager.h"

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
            INVALID,
            WAITING_ENCOUNTER,
            STARTING_ENCOUNTER,
            COMBAT_FORM,
            CACOON_FORM,
            DEFEATED,
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

        public:
            BossController(GameObject* game_object);
            ~BossController() override = default;

            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;
            
            BossState GetState() const { return state; };
            void RegisterHit(int dmg);
            void UpdateHpBar();

        private:
            void SetUpHpBar();

        private:
            SERIALIZE_FIELD(int, state_value);
            SERIALIZE_FIELD(GameObject*, hp_bar_go);
            ComponentProgressBar* hp_bar = nullptr;
            BossState state = BossState::INVALID;
            BossState prev_state = BossState::INVALID;
            int total_hp = 100;
            int current_hp = total_hp;



        };
    } // namespace Scripting
} // namespace Hachiko*/
