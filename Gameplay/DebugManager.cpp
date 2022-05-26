#include "scriptingUtil/gameplaypch.h"
#include "DebugManager.h"

#include <components/ComponentTransform.h>
#include <core/GameObject.h>

Hachiko::Scripting::DebugManager::DebugManager(GameObject* game_object)
	: Script(game_object, "DebugManager")
	, _button_back(nullptr)
	, _teleport_next_pos(nullptr)
	, _add_health(nullptr)
	, _remove_health(nullptr)
	, _toggle_invulnerable(nullptr)
	, _spawn_enemy(nullptr)
	, _toggle_performance_output(nullptr)
	, _toggle_wireframe(nullptr)
	, _toggle_show_colliders(nullptr)
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

	// register button interactions
	if (_button_back->IsSelected())
	{
		HE_LOG("_button_back pressed");
	}
	if (_teleport_next_pos->IsSelected())
	{
		HE_LOG("_teleport_next_pos pressed");
	}
	if (_add_health->IsSelected())
	{
		HE_LOG("_add_health pressed");
	}
	if (_remove_health->IsSelected())
	{
		HE_LOG("_remove_health pressed");
	}
	if (_toggle_invulnerable->IsSelected())
	{
		HE_LOG("_toggle_invulnerable pressed");
	}
	if (_spawn_enemy->IsSelected())
	{
		HE_LOG("_spawn_enemy pressed");
	}
	if (_toggle_performance_output->IsSelected())
	{
		HE_LOG("_toggle_performance_output pressed");
	}
	if (_toggle_wireframe->IsSelected())
	{
		HE_LOG("_toggle_wireframe pressed");
	}
	if (_toggle_show_colliders->IsSelected())
	{
		HE_LOG("_toggle_show_colliders pressed");
	}
	
	
}