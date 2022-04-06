#include "scriptingUtil/gameplaypch.h"
#include "SomeScript.h"
#include "components/ComponentTransform.h"

Hachiko::Scripting::SomeScript::SomeScript(GameObject* game_object)
	: Script(game_object, "SomeScript")
{

}

void Hachiko::Scripting::SomeScript::OnAwake()
{
	_initial_position = game_object->GetTransform()->GetPosition();
	_position_offset = math::float3(3.0f, 0.0f, 0.0f);
	_lerp_position = 0.0f;
}

void Hachiko::Scripting::SomeScript::OnUpdate()
{
	static const float duration = 0.5f;

	_lerp_position += Time::DeltaTime() / duration;
	_lerp_position = _lerp_position > 1.0f ? 1.0f : _lerp_position;

	math::float3 current_position = math::float3::Lerp(_initial_position,
		_initial_position + _position_offset, _lerp_position);

	game_object->GetTransform()->SetGlobalPosition(current_position);


	if (_lerp_position == 1.0f)
	{
		_lerp_position = 0.0f;

		_initial_position = current_position;

		_position_offset = -_position_offset;
	}
}