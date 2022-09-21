#include "scriptingUtil/gameplaypch.h"
#include "Stalagmite.h"

Hachiko::Scripting::Stalagmite::Stalagmite(GameObject* game_object)
	: Script(game_object, "Stalagmite")
{
}

void Hachiko::Scripting::Stalagmite::ActiveStalagmite()
{
	game_object->SetActive(true);
	crystal->SetActive(false);
}

void Hachiko::Scripting::Stalagmite::ActiveEffects()
{
	_explosion_effect->SetActive(true);
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

void Hachiko::Scripting::Stalagmite::ActiveCrystal()
{
	_explosion_effect->SetActive(false);
	GEO->SetActive(false);
	crystal->SetActive(true);
}

