#pragma once

#include <scripting/Script.h>


namespace Hachiko
{
    class GameObject;
    namespace Scripting
    {
        class BulletController : public Script
        {
            SERIALIZATION_METHODS(false)

            struct BulletStats
            {
                float lifetime = 3.f;
                float speed = 50.f;
                bool alive = false;
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

            void ShootBullet(ComponentTransform* emiter_transform);

        private:
            bool CheckCollisions(unsigned bullet_idx);
        private:
            SERIALIZE_FIELD(float, _collider_radius);
            unsigned _max_bullets = 20;
            std::vector<GameObject*> _bullets{};
            std::vector<BulletStats> _bullet_stats;
            

            math::float3 _direction;
            int _damage;
        };
    } // namespace Scripting
} // namespace Hachiko
