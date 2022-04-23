#pragma once

#include <scripting/Script.h>

namespace Hachiko
{ 
class GameObject;

namespace Scripting
{
class PlayerCamera;

class PlayerController : public Script
{
	SERIALIZATION_METHODS(false)

public:
	PlayerController(GameObject* game_object);
	~PlayerController() override = default;

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnEditor(ImGuiContext* context) override;

private:
	SERIALIZE_FIELD(float, _movement_speed);
	SERIALIZE_FIELD(float, _dash_duration);
	SERIALIZE_FIELD(float, _dash_distance);
	SERIALIZE_FIELD(bool, _is_dashing);
	SERIALIZE_FIELD(float, _dash_progress);
	SERIALIZE_FIELD(math::float3, _dash_start);
	SERIALIZE_FIELD(float, _rotation_speed);
	SERIALIZE_FIELD(bool, _is_falling);
	SERIALIZE_FIELD(float, _original_y);
	SERIALIZE_FIELD(float, _speed_y);
	SERIALIZE_FIELD(math::float3, _starting_position);
	SERIALIZE_FIELD(double, _double_field_test);
	SERIALIZE_FIELD(int, _int_field_test);
	SERIALIZE_FIELD(unsigned, _unsigned_int_field_test);
	SERIALIZE_FIELD(math::float2, _float2_field_test);
	SERIALIZE_FIELD(math::float4, _float4_field_test);
	SERIALIZE_FIELD(std::string, _string_field_test);
	SERIALIZE_FIELD(GameObject*, _game_object_field_test);
	SERIALIZE_FIELD(ComponentButton*, _component_button_test);

};
} // namespace Scripting
} // namespace Hachiko