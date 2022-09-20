#include "scriptingUtil/gameplaypch.h"
#include "Stalagmite.h"

Hachiko::Scripting::Stalagmite::Stalagmite(GameObject* game_object)
	: Script(game_object, "Stalagmite")
	,  _explosion_indicator_helper(nullptr)
	, _explosion_effect(nullptr)
{
}

void Hachiko::Scripting::Stalagmite::Falling()
{
	constexpr float fall_speed = 25.f;

	float3 _stalagmite_position = GEO
		->GetComponent<ComponentTransform>()->GetGlobalPosition();

	_stalagmite_position.y -= fall_speed * Time::DeltaTime();


	GEO->GetComponent<ComponentTransform>()
		->SetGlobalPosition(_stalagmite_position);
}
