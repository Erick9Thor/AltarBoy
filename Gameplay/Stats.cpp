#include "scriptingUtil/gameplaypch.h"
#include <Stats.h>

Hachiko::Scripting::Stats::Stats()
	:_attack_power(5)
	, _attack_cd(2)
	, _move_speed(4.0f)
	, _max_hp(10)
	, _current_hp(_max_hp)
	, _is_alive(true)
	, _god_mode(false)
{
}

Hachiko::Scripting::Stats::Stats(int _ap, int _atkcooldown, int _ms, int _maxhp)
{
	_attack_power = _ap;
	_attack_cd = _atkcooldown;
	_move_speed = _ms;
	_max_hp = _maxhp;
	_current_hp = _maxhp;
	_is_alive = true;
	_god_mode = false;
}

bool Hachiko::Scripting::Stats::IsAlive()
{
	return _is_alive;
}

void Hachiko::Scripting::Stats::RecieveDamage(int damage)
{
	if (_god_mode)	return;

	_current_hp -= damage;
	math::Clamp(_current_hp, 0, _max_hp);

	if (_current_hp <= 0)
	{
		//_is_alive = false;
		//Play dead animation
		//etc...
	}
}