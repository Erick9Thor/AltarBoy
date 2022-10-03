#include "scriptingUtil/gameplaypch.h"
#include "Stalagmite.h"

Hachiko::Scripting::Stalagmite::Stalagmite(GameObject* game_object)
	: Script(game_object, "Stalagmite")
{
}

void Hachiko::Scripting::Stalagmite::ActiveStalagmite()
{
	game_object->SetActive(true);
	game_object->ChangeDissolveProgress(1.0f, true);
}

void Hachiko::Scripting::Stalagmite::ActiveEffects()
{
	_explosion_effect->SetActive(true);
}

void Hachiko::Scripting::Stalagmite::Falling(float fall_progress)
{
	float3 _stalagmite_position = GEO
		->GetComponent<ComponentTransform>()->GetLocalPosition();

	_stalagmite_position.y = Lerp(25.f, 0.f, fall_progress);


	GEO->GetComponent<ComponentTransform>()
		->SetLocalPosition(_stalagmite_position);
}

void Hachiko::Scripting::Stalagmite::SetBlockInPlace()
{
	_blocking_object->GetTransform()->SetGlobalPosition(game_object->GetTransform()->GetGlobalPosition());
}

void Hachiko::Scripting::Stalagmite::Dissolved()
{
	_blocking_object->GetTransform()->SetGlobalPosition(float3(0.0f, -50.0f, 0.0f));
	game_object->SetActive(false);
}

