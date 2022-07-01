#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"
#include "entities/enemies/BugState.h"

namespace Hachiko
{
    class GameObject;
    class ComponentTransform;

    namespace Scripting
    {
        class AudioManager;
        class PlayerController;
        class EnemyController : public Script
        {
            SERIALIZATION_METHODS(false)

        public:
            EnemyController(GameObject* game_object);
            ~EnemyController() override = default;

            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;
            
            BugState GetState() const;

            const Stats* GetStats();
            bool IsAlive() { return _combat_stats->IsAlive(); };
            void RegisterHit(int player_atk, math::float3 direction, float knockback);
            void GetParasite();

            void CheckState();
            void ResetEnemy();
            void ResetEnemyPosition();

        private:
            void Attack();
            void ChasePlayer();
            void GoBack();
            void RecieveKnockback();
            void Stop();

            void MoveInNavmesh();
            void DropParasite();
            void DestroyEntity();

        private:
            Stats* _combat_stats;
            SERIALIZE_FIELD(int, _aggro_range);
            SERIALIZE_FIELD(int, _attack_range);

            SERIALIZE_FIELD(float3, _spawn_pos);
            SERIALIZE_FIELD(bool, _spawn_is_initial);
            SERIALIZE_FIELD(GameObject*, _player);
            SERIALIZE_FIELD(GameObject*, _enemy_body);
            SERIALIZE_FIELD(GameObject*, _parasite);
            SERIALIZE_FIELD(GameObject*, _audio_manager_game_object);
            
            SERIALIZE_FIELD(BugState, _state);
            SERIALIZE_FIELD(BugState, _previous_state);
            SERIALIZE_FIELD(bool, _already_in_combat);

            AudioManager* _audio_manager;
            PlayerController* _player_controller;
            ComponentTransform* transform;
            ComponentAnimation* animation;
            
            ComponentAudioSource* _audio_source;

            Quat _spawn_rot;

            math::float3 _player_pos;
            math::float3 _target_pos;
            math::float3 _current_pos;
            float _attack_cooldown;

            SERIALIZE_FIELD(float, _attack_animation_duration);
            SERIALIZE_FIELD(float, _attack_animation_timer);

            SERIALIZE_FIELD(bool, _is_ranged_attack);

            bool _is_stunned = false;
            float _stun_time = 0.0f;
            float _acceleration = 0.0f;
            float _speed = 0.0f;
            float _parasite_lifespan = 5.0f;
            float _current_lifetime = 0.0f;
            float3 _knockback_pos = float3::zero;

        };
    } // namespace Scripting
} // namespace Hachiko*/
