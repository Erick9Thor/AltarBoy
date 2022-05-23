#pragma once

#include <scripting/Script.h>
#include "Stats.h"

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

            Stats& GetStats();
            void RegisterPlayerHit(int player_atk);

        private:
            void Attack();
            void ChasePlayer();
            void GoBack();

            void Move();
            void MoveInNavmesh();

            void DestroyEntity();

        private:
            SERIALIZE_FIELD(Stats, _combat_stats);

            SERIALIZE_FIELD(int, _aggro_range);
            SERIALIZE_FIELD(int, _attack_range);
            SERIALIZE_FIELD(float3, _spawn_pos);

            SERIALIZE_FIELD(GameObject*, _player);;
            PlayerController* _player_controller;
            ComponentTransform* transform;
            math::float3 _player_pos;
            math::float3 _target_pos;
            math::float3 _current_pos;
            float _attack_cooldown;
        };
    } // namespace Scripting
} // namespace Hachiko*/
