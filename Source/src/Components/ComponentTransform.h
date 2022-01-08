#pragma once
#include "Component.h"
#include "../GameObject.h"

#include "MathGeoLib.h"

class GameObject;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* new_object, float3 position = float3::zero, Quat rotation = Quat::identity, float3 scale = float3::one);
	ComponentTransform(GameObject* new_object, const float4x4& transform = float4x4::identity);
	~ComponentTransform();
	static inline Type GetType()
	{
		return Type::Transform;
	};

	void SetPosition(float3 new_position);
	void SetRotationAxis(float3 x, float3 y, float3 z);

	void SetLocalTransform(float4x4 new_transform);
	void SetLocalTransform(float3 position, Quat rotation, float3 scale);

	inline void SetLocalPosition(float3 new_position)
	{
		SetLocalTransform(new_position, local_rotation, local_scale);
	}
	inline void SetLocalScale(float3 new_scale)
	{
		SetLocalTransform(local_position, local_rotation, new_scale);
	}
	inline void SetLocalRotation(Quat new_rotation)
	{
		SetLocalTransform(local_position, new_rotation, local_scale);
	}
	void SetLocalRotation(float3 rotation_angles);

	void SetGlobalTransform(float4x4 transform);
	void UpdateGlobalTransformHierarchy();

	inline float4x4 GetTransform() const
	{
		return transform;
	}

	inline float3 GetPosition() const
	{
		return transform.TranslatePart();
	}
	inline float3 GetFwd() const
	{
		return transform.WorldZ();
	}
	inline float3 GetUp() const
	{
		return transform.WorldY();
	}
	inline float3 GetRight() const
	{
		return transform.WorldX();
	}

	void LookAt(float3 target, float3 world_up = float3::unitY);

	void OnTransformUpdated() override
	{
		changed = false;
	}

	void DrawGui() override;
	inline bool HasChanged() const
	{
		return changed;
	}

private:
	bool changed = true;

	float4x4 transform;

	float4x4 local_transform;
	float3 local_position;
	float3 local_scale;
	Quat local_rotation;
	float3 local_rotation_euler;
};
