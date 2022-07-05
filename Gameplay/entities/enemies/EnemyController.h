#pragma once

#include "entities/Stats.h"

namespace Hachiko
{
    class GameObject;
    class ComponentTransform;
    class ComponentParticleSystem;

    namespace Scripting
    {
        class AudioManager;
        class PlayerController;
        class CombatManager;
        class PlayerCamera;

        enum class BugState
        {
            INVALID,
            DEAD,
            IDLE,
            MOVING,
            MOVING_BACK,
            ATTACKING,
            PARASITE,
            SPAWNING,
            PATROL
        };

        enum class WormState
        {
            INVALID,
            DEAD,
            IDLE,
            HIDEN,
            ATTACKING,
            PARASITE,
            SPAWNING,
            HIT
        };

        class EnemyController
        {

        public:
            EnemyController() {}

            virtual ~EnemyController() = default;

            virtual void OnAwake() {};
            virtual void OnStart() {};
            virtual void OnUpdate() {};
            virtual GameObject* GetGameObject() { return nullptr; };
            virtual Stats* GetStats() { return nullptr; };
            virtual bool IsAlive() { return false; };
            virtual void SetIsFromGauntlet(bool v) {}
            virtual void RegisterHit(int player_atk, math::float3 direction, float knockback, bool is_from_player) {};
            virtual void GetParasite() {};
            virtual void Spawn() {};
            virtual bool ParasiteDropped() { return false; };

            virtual void CheckState() {};
            virtual void ResetEnemy() {};
            virtual void ResetEnemyPosition() {};
        protected:
            Stats* _combat_stats = nullptr;
        };
    } // namespace Scripting
} // namespace Hachiko*/
