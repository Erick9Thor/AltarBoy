#include "scriptingUtil/gameplaypch.h"
#include "DebugManager.h"

#include <components/ComponentTransform.h>
#include <core/GameObject.h>

Hachiko::Scripting::DebugManager::DebugManager(GameObject* game_object)
	: Script(game_object, "DebugManager")
	, _button_back(nullptr)
{
}

void Hachiko::Scripting::DebugManager::OnAwake()
{
	is_active = false;
	_is_god_mode = false;
}

void Hachiko::Scripting::DebugManager::OnStart()
{
	for (GameObject* child : game_object->children)
	{
		child->SetActive(is_active);
	}
}

void Hachiko::Scripting::DebugManager::OnUpdate()
{
	if (Input::IsKeyDown(Input::KeyCode::KEY_F3))
	{
		is_active = !is_active;
		for (GameObject* child : game_object->children)
		{
			child->SetActive(is_active);
		}
	}
	if (Input::IsKeyDown(Input::KeyCode::KEY_G) && is_active)
	{
		_is_god_mode = !_is_god_mode;
	}
}