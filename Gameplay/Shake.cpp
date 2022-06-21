#include "scriptingUtil/gameplaypch.h"
#include "Shake.h"

#include <components/ComponentTransform.h>


Hachiko::Scripting::Shake::Shake(GameObject* game_object)
	: Script(game_object, "Shake")
	, _shake_time(0.8f)
	, _shake_intensity(0.2f)
	, _shake_magnitude(1.0f)
{
}

void Hachiko::Scripting::Shake::OnAwake()
{
}

void Hachiko::Scripting::Shake::OnStart()
{
	transform = game_object->GetTransform();
}

void Hachiko::Scripting::Shake::OnUpdate()
{
	
	ComponentTransform* transform = game_object->GetTransform();
	math::float3 current_position = transform->GetGlobalPosition();

	float3 shake_offset = float3::zero;

	if (shake_elapsed < _shake_time)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float x = (r - 0.5f) * _shake_magnitude * _shake_intensity;
		r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = (r - 0.5f) * _shake_magnitude * _shake_intensity;

		shake_elapsed += Time::DeltaTime();
		_shake_magnitude = (1 - (shake_elapsed / _shake_time)) * (1 - (shake_elapsed / _shake_time));
		shake_offset = float3(x, 0, z);
	}
	else
	{
		_shake_time = 0.0f;
		shake_elapsed = 0.0f;
		shake_offset = float3::zero;
	}

	transform->SetGlobalPosition(current_position + shake_offset);
}

