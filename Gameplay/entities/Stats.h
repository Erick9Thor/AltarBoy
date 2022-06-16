#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
    class GameObject;
    namespace Scripting
    {
        class Stats : public Script
        {
            SERIALIZATION_METHODS(false)

        public:
            Stats(GameObject* game_object);
            ~Stats() override = default;

            void ReceiveDamage(int _damage);
            bool IsAlive();
        public:
            SERIALIZE_FIELD(int, _attack_power);
            SERIALIZE_FIELD(int, _attack_cd);
            SERIALIZE_FIELD(float, _attack_range);
            SERIALIZE_FIELD(float, _move_speed);
            SERIALIZE_FIELD(int, _max_hp);
            bool is_alive;
            int _current_hp;
        };
    } // namespace Scripting
} // namespace Hachiko*/