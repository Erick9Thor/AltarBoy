#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
class GameObject;
namespace Scripting
{
class PlayerController;
class PlayerCamera : public Script
{
	SERIALIZATION_METHODS(false)

public:
	PlayerCamera(GameObject* game_object);
	~PlayerCamera() override = default;

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate() override;

	void Shake(float time, float intesity);
	void ChangeRelativePosition(math::float3 new_rel_pos, float time = -1.0f);
	void RevertRelativePosition();
	void SwitchRelativePosition(math::float3 new_rel_pos);

private:
	void CheckForObjective();
	void SetLookAhead();
	void MoveCamera();
	void RecalculateRelativePos();

	float2 MoveCameraWithMouse();
	void ScrollWheelZoom(float3* cam_pos);
	float3 Shake();

private:
	SERIALIZE_FIELD(math::float3, _relative_position_to_player);
	SERIALIZE_FIELD(GameObject*, _objective);
	SERIALIZE_FIELD(float, _follow_delay);

	math::float3 _look_ahead = math::float3::zero;
	PlayerController* _player_ctrl = nullptr;
	GameObject* _current_objective = nullptr;
	math::float3 _relative_pos_aux = math::float3::zero;
	bool _is_temporary_moved = false;
	float _position_timer = 0.0f;
	float shake_elapsed = 0.0f;
	float shake_time = 0.0f;
	float shake_intensity = 0.0f;
	float shake_magnitude = 0.0f;
};
} // namespace Scripting
} // namespace Hachiko