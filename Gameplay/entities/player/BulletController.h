#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
    class GameObject;
    class ComponentTransform;

    namespace Scripting
    {
        class EnemyController;
        class CrystalExplosion;

        class BulletController : public Script
        {
            SERIALIZATION_METHODS(false);

            struct BulletStats
            {
                friend BulletController;
                float charge_time = 1.f;
                float lifetime = 3.f;
                float size = 1.f;
                float speed = 50.f;
                float damage = 1.f;
                float GetChargedPercent();
            private:
                bool alive = false;
                float current_charge = 0.f;
                float3 direction = float3::zero;
                float3 prev_position = float3::zero;
            };
            
        public:
            BulletController(GameObject* game_object);
            ~BulletController() override;
            
            void OnAwake() override;
            void OnUpdate() override;

            void SetDamage(int new_damage);
            void SetForward(float3 new_forward);

            unsigned* ShootBullet(ComponentTransform* emitter_transform, BulletStats new_stats);
            void StopBullet(unsigned bullet_index);
            const BulletStats& GetBulletStats(unsigned bullet_idx) const;

        private:
            void UpdateBulletTrajectory(unsigned bullet_idx);
            bool CheckCollisions(unsigned bullet_idx);
            void ActivateBullet(unsigned bullet_idx);
            void DeactivateBullet(unsigned bullet_idx);
            void SetBulletTrajectory(ComponentTransform* emitter_transform, unsigned bullet_idx);

            // Processes hit and returns hit distance to check what to damage
            EnemyController* ProcessEnemies(GameObject* bullet, float bullet_size, LineSegment& trajectory, float& closest_hit);
            CrystalExplosion* ProcessObstacles(GameObject* bullet, float bullet_size, LineSegment& trajectory, float& closest_hit);
        private:
            unsigned _max_bullets = 5;
            std::vector<GameObject*> _bullets{};
            std::vector<BulletStats> _bullet_stats;
            

            math::float3 _direction;
            int _damage;
        };
    } // namespace Scripting
} // namespace Hachiko
