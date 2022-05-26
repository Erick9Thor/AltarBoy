#include "scriptingUtil/gameplaypch.h"
#include "FancyLights.h"

Hachiko::Scripting::FancyLights::FancyLights(Hachiko::GameObject* game_object)
	: Script(game_object, "FancyLights")
	, _radius(0.0f)
	, _distance(0.0f)
	, _speed(0.0f)
	, _angle(0.0f)
	, _rotate_on_y(false)
	, _point_light(nullptr)
{
}

void Hachiko::Scripting::FancyLights::OnAwake()
{
	_point_light = game_object->GetComponent<ComponentPointLight>();

	math::LCG lcg; 

	_point_light->color = float4::RandomGeneral(lcg, 0.0f, 1.0f);
}

void Hachiko::Scripting::FancyLights::OnUpdate()
{
	float velocity = Time::DeltaTime() * _speed;
	_angle += velocity;

	math::float3 center = game_object->GetTransform()->GetGlobalPosition();

	float x = std::sin(_angle) * _distance + center.x;
	float y = std::sin(_angle) * _distance + center.y;

	if (_rotate_on_y)
	{
		game_object->GetTransform()->SetGlobalPosition(math::float3(x, y, center.z + _distance));
	}
	else
	{
		game_object->GetTransform()->SetGlobalPosition(math::float3(x, center.y + _distance, y));
	}
}