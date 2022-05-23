#include "scriptingUtil/gameplaypch.h"

#include "Stats.h"

#include "CrystalExplotion.h"


Hachiko::Scripting::CrystalExplotion::CrystalExplotion(GameObject* game_object)
	: Script(game_object, "CrystalExplotion")
	, _stats(0, 0, 0, 5)
{
}

void Hachiko::Scripting::CrystalExplotion::OnAwake()
{
	_state = CristalState::IDLE;
}

void Hachiko::Scripting::CrystalExplotion::OnStart()
{
}

void Hachiko::Scripting::CrystalExplotion::OnUpdate()
{
	if (!_stats.IsAlive())
	{
		return;
	}

	if (_stats._current_hp <= 0)
	{
		DestroyCristall();
	}
}


void Hachiko::Scripting::CrystalExplotion::ReceiveDamage(int damage)
{
	_stats.ReceiveDamage(damage);
}

void Hachiko::Scripting::CrystalExplotion::DestroyCristall()
{
	_animator->StartAnimating(_crashing_index, false, 200);
	_stats._is_alive = false;

	// Wait until animation finish

	// delete game_object;
}
