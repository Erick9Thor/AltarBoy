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
            Stats(int _ap, int _as, int _ms, int _maxhp);
            ~Stats() = default;

            void RecieveDamage(int _damage);
            bool IsAlive();
        public:
            int _attack_power;
            int _attack_cd;
            float _move_speed;

            int _max_hp;
            int _current_hp;

            bool _is_alive;
            bool _god_mode;
        };
    } // namespace Scripting
} // namespace Hachiko*/