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
	_obstacle_comp = _obstacle->GetComponent<ComponentObstacle>();
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

	if (fall_progress == 1.f)
	{
		_obstacle_comp->AddObstacle();
	}
}

void Hachiko::Scripting::Stalagmite::Dissolved()
{

	_obstacle_comp->RemoveObstacle();

	_stalagmite_collapsed = false;

	// After it being dissolved we move the GEO out of the way

	float3 _stalagmite_position = GEO
		->GetComponent<ComponentTransform>()->GetLocalPosition();

	_stalagmite_position.y = 25.f;

	GEO->GetComponent<ComponentTransform>()
		->SetLocalPosition(_stalagmite_position);

	game_object->SetActive(false);
}

