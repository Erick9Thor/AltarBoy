#pragma once

#include "entities/enemies/EnemyController.h"
#include "entities/player/CombatManager.h"

namespace Hachiko
{
    namespace Scripting
    {
        class WormController : public Script, public EnemyController
        {
            SERIALIZATION_METHODS(false)

        public:
            WormController(GameObject* game_object);
            ~WormController() override = default;

            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;

            WormState GetState() const;

            //const Stats* GetStats();
            bool IsAlive() override { return _combat_stats->IsAlive(); };
            //void RegisterHit(int player_atk, math::float3 direction, float knockback, bool is_from_player);
            //void GetParasite();
            void Spawn() override;
            //bool ParasiteDropped() { return _parasite_dropped; };

            void CheckState() override;
            //void ResetEnemy();
            //void ResetEnemyPosition();

        private:
            void SpawnController();
            //void DeathController();
            //void StunController();
            //void AttackController();
            //void IdleController();

            //void Attack();
            //void GoBack();
            //void RecieveKnockback();

            //void DropParasite();
            //void DestroyEntity();

            //float4x4 GetMeleeAttackOrigin(float attack_range) const;
            //bool IsAttacking() const
            //{
            //    return _state == BugState::ATTACKING;
            //}

        private:
            //SERIALIZE_FIELD(int, _aggro_range);
            //SERIALIZE_FIELD(float, _attack_range);
            //SERIALIZE_FIELD(float, _attack_delay);
            SERIALIZE_FIELD(float, _spawning_time);
            //SERIALIZE_FIELD(float, _idle_cooldown);
            //SERIALIZE_FIELD(float3, _spawn_pos);
            //SERIALIZE_FIELD(bool, _spawn_is_initial);
            //SERIALIZE_FIELD(GameObject*, _player);
            SERIALIZE_FIELD(GameObject*, _enemy_body);
            //SERIALIZE_FIELD(GameObject*, _parasite);
            //SERIALIZE_FIELD(GameObject*, _audio_manager_game_object);
            SERIALIZE_FIELD(GameObject*, _small_dust);
            SERIALIZE_FIELD(GameObject*, _big_dust);

            //SERIALIZE_FIELD(bool, _already_in_combat);

            //SERIALIZE_FIELD(bool, _is_from_gautlet);

            //AudioManager* _audio_manager;
            PlayerController* _player_controller = nullptr;
            PlayerCamera* _player_camera = nullptr;
            ComponentTransform* transform = nullptr;
            ComponentAnimation* animation = nullptr;
            CombatManager* _combat_manager;
            //ComponentAudioSource* _audio_source;
            ComponentParticleSystem* _small_dust_particles = nullptr;
            ComponentParticleSystem* _big_dust_particles = nullptr;

            //Quat _spawn_rot;

            //math::float3 _player_pos;
            //math::float3 _target_pos;
            //math::float3 _current_pos;
            //float _attack_cooldown;

            SERIALIZE_FIELD(float, _attack_animation_duration);
            SERIALIZE_FIELD(float, _attack_animation_timer);

            WormState _state = WormState::SPAWNING;
            WormState _previous_state = WormState::INVALID;
            //bool _parasite_dropped = false;
            //bool _is_stunned = false;
            //float _stun_time = 0.0f;
            //float _acceleration = 0.0f;
            //float _speed = 0.0f;
            //float _current_idle_cooldown = 0.0f;
            //float _enraged = 0.0f;
            //float _enemy_dissolve_time = 3.0f;
            //float _enemy_dissolving_time_progress = 0.0f;
            //const float _enemy_dissolving = 1 / math::Sqrt(_enemy_dissolve_time);
            //float _parasite_dissolve_time = 10.0f;
            //float _parasite_dissolving_time_progress = 0.0f;
            //const float _parasite_dissolving = 1 / math::Sqrt(_parasite_dissolve_time);
            //float3 _knockback_pos = float3::zero;
            //float _attack_current_delay = 0.f;
            float _current_spawning_time = 0.0f;
            bool _has_spawned = false;

            // Special attacks
            CombatManager::AttackStats push_attack;
        };
    } // namespace Scripting
} // namespace Hachiko*/