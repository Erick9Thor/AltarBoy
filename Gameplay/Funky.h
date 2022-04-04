#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
	class GameObject;
	class ComponentPointLight;

namespace Scripting
{
class Funky : public Script
{
	SERIALIZATION_METHODS(false)

public:
	Funky(GameObject* game_object);
	~Funky() override = default;

	void OnStart() override;
	void OnUpdate() override;

private:
	SERIALIZE_FIELD(int, _lerp_index);
	SERIALIZE_FIELD(float, _lerp_position);
	SERIALIZE_FIELD(bool, _lerp_reverse);
	SERIALIZE_FIELD(math::float4, _initial_color);
	ComponentPointLight* _point_light;
};
}
}