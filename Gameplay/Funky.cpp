#include "scriptingUtil/gameplaypch.h"
#include "Funky.h"
#include <components/ComponentPointLight.h>

Hachiko::Scripting::Funky::Funky(GameObject* game_object)
	: Script(game_object, "Funky")
	, _lerp_index(0)
	, _lerp_position(0.0f)
	, _lerp_reverse(false)
{
	// NOTE(Baran): Use of Script constructor this way will be strictly 
	// prohibited as we cannot use SEH for object creation and any nullptr
	// exception will cause a crash in the engine. This code is purely for
	// testing purposes before the implementation of play-mode and OnAwake
	// call.

	_point_light = game_object->GetComponent<ComponentPointLight>();


	if (_point_light != nullptr)
	{
		_initial_color = _point_light->color;
	}
}

void Hachiko::Scripting::Funky::OnStart()
{
	
}

void Hachiko::Scripting::Funky::OnUpdate()
{
	static const float duration = 0.5f;

	_lerp_position += Time::DeltaTime() / duration;
	_lerp_position = _lerp_position > 1.0f ? 1.0f : _lerp_position;

	math::float4 final_color = _initial_color;
	final_color[_lerp_index] = _lerp_reverse ? 0.0f : 1.0f;

	math::float4 current_color = math::float4::Lerp(_initial_color, 
		final_color, _lerp_position);

	_point_light->color = current_color;

	if (_lerp_position == 1.0f)
	{
		_lerp_position = 0.0f;

		_lerp_index = (_lerp_index + 1) % 3;

		if (_lerp_index == 0)
		{
			_lerp_reverse = !_lerp_reverse;
		}

		_initial_color = current_color;
	}
}