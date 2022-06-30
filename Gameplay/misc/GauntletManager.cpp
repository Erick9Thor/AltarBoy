#include "scriptingUtil/gameplaypch.h"

#include "GauntletManager.h"

Hachiko::Scripting::GauntletManager::GauntletManager(GameObject* game_object)
	: Script(game_object, "GauntletManager")
	, _trigger_radius(0.f)
	, _door_1(nullptr)
	, _door_2(nullptr)
	, _pack_1(nullptr)
	, _pack_2(nullptr)
	, _pack_3(nullptr)
{}

void Hachiko::Scripting::GauntletManager::OnAwake()
{
	_doors.clear();
	if (_door_1) _doors.push_back(_door_1);
	if (_door_2) _doors.push_back(_door_2);
	
	_enemy_packs.clear();
	if (_pack_1) _enemy_packs.push_back(_pack_1);
	if (_pack_2) _enemy_packs.push_back(_pack_2);
	if (_pack_3) _enemy_packs.push_back(_pack_3);

	CloseDoors();

	if (!_use_trigger) started = true;
}

void Hachiko::Scripting::GauntletManager::OnUpdate()
{
	if (completed) return;
	
	if (!started)
	{
		if (_trigger_radius >= game_object->GetTransform()->GetGlobalPosition().Distance(_combat_manager->GetPlayer()->GetTransform()->GetGlobalPosition()))
		{
			// Here we would start spawning the first round
		}
	}
	else
	{
		CheckRoundStatus();
	}

}

void Hachiko::Scripting::GauntletManager::CheckRoundStatus()
{
	if(!_combat_manager->IsPackDead(_enemy_packs[current_round])) return;
	++current_round;
	if (current_round < _enemy_packs.size()) return;
	completed = true;

	OpenDoors();
}

void Hachiko::Scripting::GauntletManager::OpenDoors()
{
	for (DoorController* door : _doors)
	{
		door->Open();
	}
}

void Hachiko::Scripting::GauntletManager::CloseDoors()
{
	for (DoorController* door : _doors)
	{
		door->Close();
	}
}
