#pragma once

#include <scripting/Script.h>
#include "Stats.h"

namespace Hachiko
{
    class GameObject;
    namespace Scripting
    {
        class BulletController : public Script
        {
            SERIALIZATION_METHODS(false)

        public:
            BulletController();
            ~BulletController() override = default;

            void OnAwake() override;
            void OnUpdate() override;

        private:
            bool CheckCollisions();
        private:
            SERIALIZE_FIELD(float, _move_speed);
            SERIALIZE_FIELD(float, _lifetime);
            SERIALIZE_FIELD(float, _collider_radius);

            math::float3 _direction;
            float _destroy_time;
            int _damage;
        };
    } // namespace Scripting
} // namespace Hachiko*/
