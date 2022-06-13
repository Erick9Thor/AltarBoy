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
                float lifetime = FLT_MAX;
                float speed = 0.1f;
                bool alive = false;
            };
        public:
            BulletController(GameObject* game_object);
            ~BulletController() override = default;
            
            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;

            void SetDamage(int new_damage);
            void SetForward(float3 new_forward);

        private:
            bool CheckCollisions();
        private:
            SERIALIZE_FIELD(float, _move_speed);
            SERIALIZE_FIELD(float, _lifetime);
            SERIALIZE_FIELD(float, _collider_radius);
            unsigned _max_bullets = 5;
            std::vector<GameObject*> _bullets{};
            std::vector<BulletStats> _bullet_stats;

            math::float3 _direction;
            int _damage;
        };
    } // namespace Scripting
} // namespace Hachiko
