#pragma once

#include <scripting/Script.h>
#include "Stats.h"
#include "PlayerState.h"

namespace Hachiko
{
    class GameObject;
    class ComponentTransform;
    namespace Scripting
    {
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

            Stats& GetStats();
            void ReceiveDamage(int damage, math::float3 direction);

        private:
            void Attack();
            void ChasePlayer();
            void GoBack();
            void RecieveKnockback();
            void Stop();

            void Move();
            void MoveInNavmesh();

            void DestroyEntity();

        public: 
            SERIALIZE_FIELD(Stats, _stats);

        private:
            SERIALIZE_FIELD(int, _aggro_range);
            SERIALIZE_FIELD(int, _attack_range);
            SERIALIZE_FIELD(float3, _spawn_pos);
            SERIALIZE_FIELD(bool, _spawn_is_initial);
            SERIALIZE_FIELD(GameObject*, _player);
            SERIALIZE_FIELD(BugState, _state);

            PlayerController* _player_controller;
            ComponentTransform* transform;
            math::float3 _player_pos;
            math::float3 _target_pos;
            math::float3 _current_pos;
            float _attack_cooldown;

            SERIALIZE_FIELD(float, _attack_animation_duration);
            SERIALIZE_FIELD(float, _attack_animation_timer);

            bool _is_stunned = false;
            float _stun_time = 0.0f;
            float _acceleration = 0.0f;
            float _speed = 0.0f;
            float3 _knockback_pos = float3::zero;

        };
    } // namespace Scripting
} // namespace Hachiko*/
