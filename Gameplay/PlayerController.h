#pragma once

#include <scripting/Script.h>

namespace Hachiko
{ 
class GameObject;
namespace Scripting
{
class PlayerController : public Script
{
	SERIALIZATION_METHODS(false)

public:
	PlayerController(GameObject* game_object);
	~PlayerController() override = default;

	void Start() override;
	void Update() override;

private:
	SERIALIZE_FIELD(int, _integer_field);
	SERIALIZE_FIELD(std::string, _string_field);
};
}
}