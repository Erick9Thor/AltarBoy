#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
    namespace Scripting
    {
        class Stats
        {
        public:
            Stats();
            Stats(int _ap, int _as, int _ms, float _maxhp);
            ~Stats() = default;

            void ReceiveDamage(int _damage);
            bool IsAlive();
        public:
            int _attack_power;
            int _attack_cd;
            float _move_speed;

            float _max_hp;
            float _current_hp;

            bool _is_alive;
            bool _god_mode;
        };
    } // namespace Scripting
} // namespace Hachiko*/