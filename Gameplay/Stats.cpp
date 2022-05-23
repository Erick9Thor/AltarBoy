#include "scriptingUtil/gameplaypch.h"
#include <Stats.h>

Hachiko::Scripting::Stats::Stats()
	: _attack_power(1)
	, _attack_cd(2)
	, _move_speed(7.0f)

	, _max_hp(3)
	, _current_hp(_max_hp)
{
}

Hachiko::Scripting::Stats::Stats(int _ap, int _atkcooldown, int _ms, int _maxhp)
{
	_attack_power = _ap;
	_attack_cd = _atkcooldown;
	_move_speed = _ms;
	_max_hp = _maxhp;
	_current_hp = _maxhp;
}

bool Hachiko::Scripting::Stats::IsAlive()
{
	return _current_hp > 0;
}

void Hachiko::Scripting::Stats::ReceiveDamage(int damage)
{
	_current_hp -= damage;
	math::Clamp(_current_hp, 0, _max_hp);
}