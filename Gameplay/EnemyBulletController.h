#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
    class GameObject;
    namespace Scripting
    {
        class EnemyBulletController : public Script
        {
            SERIALIZATION_METHODS(false)

        public:
            EnemyBulletController(GameObject* game_object);
            ~EnemyBulletController() override = default;

            void OnAwake() override;
            void OnUpdate() override;

            void SetTarget(GameObject* player);
            void SetDamage(int new_damage);
            void SetForward(float3 new_forward);

        private:
            bool CheckCollisions();
        private:
            SERIALIZE_FIELD(float, _move_speed);
            SERIALIZE_FIELD(float, _lifetime);
            SERIALIZE_FIELD(float, _collider_radius);

            GameObject* _player;
            math::float3 _direction;
            int _damage;
        };
    } // namespace Scripting
} // namespace Hachiko
