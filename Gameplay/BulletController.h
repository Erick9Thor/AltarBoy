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
                float lifetime = 10.f;
                float speed = 0.1f;
                bool alive = false;
                float3 direction = float3(0.f, 1.f, 1.f);
            };
        public:
            BulletController(GameObject* game_object);
            ~BulletController() override = default;
            
            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;

            void SetDamage(int new_damage);
            void SetForward(float3 new_forward);

            void ShootBullet(ComponentTransform* emiter_transform);

        private:
            bool CheckCollisions();
        private:
            SERIALIZE_FIELD(float, _collider_radius);
            unsigned _max_bullets = 5;
            std::vector<GameObject*> _bullets{};
            std::vector<BulletStats> _bullet_stats;
            

            math::float3 _direction;
            int _damage;
        };
    } // namespace Scripting
} // namespace Hachiko
