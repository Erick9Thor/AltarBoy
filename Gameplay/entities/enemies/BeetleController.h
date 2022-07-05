#pragma once

#include <entities/enemies/EnemyController.h>
#include <scripting/Script.h>

namespace Hachiko
{
    namespace Scripting
    {
        class BeetleController : public Script, public EnemyController
        {
            SERIALIZATION_METHODS(false)

        public:
            BeetleController(GameObject* game_object);
            ~BeetleController() override = default;

            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;
            
            BugState GetState() const;

            Stats* GetStats() override;
            bool IsAlive() override { return _combat_stats->IsAlive(); };
            void SetIsFromGauntlet(bool v) override { _is_from_gautlet = v; }
            void RegisterHit(int player_atk, math::float3 direction, float knockback, bool is_from_player) override;
            void GetParasite() override;
            void Spawn() override;
            bool ParasiteDropped() override { return _parasite_dropped; };

            void CheckState() override;
            void ResetEnemy() override;
            void ResetEnemyPosition() override;

        private:
            void SpawnController();
            void DeathController();
            void StunController();
            void AttackController();
            void IdleController();

            void Attack();
            void ChasePlayer();
            void GoBack();
            void RecieveKnockback();
            void Stop();

            void MoveInNavmesh();
            void PatrolMovement();
            void DropParasite();
            void DestroyEntity();

            float4x4 GetMeleeAttackOrigin(float attack_range) const;
            bool IsAttacking() const
            {
                return _state == BugState::ATTACKING;
            }

        private:
            SERIALIZE_FIELD(int, _aggro_range);
            SERIALIZE_FIELD(float, _attack_range);
            SERIALIZE_FIELD(float, _attack_delay);
            SERIALIZE_FIELD(float, _idle_cooldown);
            SERIALIZE_FIELD(float3, _spawn_pos);
            SERIALIZE_FIELD(bool, _spawn_is_initial);
            SERIALIZE_FIELD(GameObject*, _player);
            SERIALIZE_FIELD(GameObject*, _enemy_body);
            SERIALIZE_FIELD(GameObject*, _parasite);
            SERIALIZE_FIELD(GameObject*, _audio_manager_game_object);
            
            SERIALIZE_FIELD(BugState, _state);
            SERIALIZE_FIELD(BugState, _previous_state);
            SERIALIZE_FIELD(bool, _already_in_combat);

            SERIALIZE_FIELD(bool, _is_from_gautlet);

            AudioManager* _audio_manager = nullptr;
            PlayerController* _player_controller = nullptr;
            ComponentTransform* transform = nullptr;
            ComponentAnimation* animation = nullptr;
            CombatManager* _combat_manager = nullptr;
            ComponentAudioSource* _audio_source = nullptr;

            Quat _spawn_rot = math::Quat::identity;

            math::float3 _player_pos;
            math::float3 _target_pos;
            math::float3 _current_pos;
            float _attack_cooldown;

            SERIALIZE_FIELD(float, _attack_animation_duration);
            SERIALIZE_FIELD(float, _attack_animation_timer);

            SERIALIZE_FIELD(bool, _is_ranged_attack);

            bool _parasite_dropped = false;
            bool _is_stunned = false;
            float _stun_time = 0.0f;
            float _acceleration = 0.0f;
            float _speed = 0.0f;
            float _current_idle_cooldown = 0.0f;
            float _enraged = 0.0f;
            float _enemy_dissolve_time = 3.0f;
            float _enemy_dissolving_time_progress = 0.0f;
            const float _enemy_dissolving = 1 / math::Sqrt(_enemy_dissolve_time);
            float _parasite_dissolve_time = 10.0f;
            float _parasite_dissolving_time_progress = 0.0f;
            const float _parasite_dissolving = 1 / math::Sqrt(_parasite_dissolve_time);
            float3 _knockback_pos = float3::zero;
            float _attack_current_delay = 0.f;
            float spawning_time = 2.0f; // Once we have spawning animations this will be calculated by the animation duration
            float spawning_time_passed = 0.f;
            bool _has_spawned = false;
        };
    } // namespace Scripting
} // namespace Hachiko*/
