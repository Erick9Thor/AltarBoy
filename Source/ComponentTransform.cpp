#include "ComponentTransform.h"
#include "Component.h"
#include "GameObject.h"
#include "Globals.h"

ComponentTransform::ComponentTransform(GameObject* new_GameObject, const float4x4& transform) :
	Component(Component::Type::Transform, new_GameObject)
{
	SetLocalTransform(transform);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetLocalTransform(float4x4 newTransform)
{
	local_transform = newTransform;
	local_transform.Decompose(local_position, local_rotation, local_scale);
	local_rotation_euler = local_rotation.ToEulerXYZ() * RADTODEG;
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	local_transform = transform;
	if (gameObject->parent)
	{
		local_transform = gameObject->parent->GetComponent<ComponentTransform>()->GetTransform().Transposed() * transform;
	}
	SetLocalTransform(local_transform);
}

void ComponentTransform::SetRotationAxis(float3 x, float3 y, float3 z)
{
	transform.SetCol3(0, x);
	transform.SetCol3(1, y);
	transform.SetCol3(2, z);

	SetGlobalTransform(transform);
}

void ComponentTransform::LookAt(float3 target, float3 worldUp)
{
	float3 direction = target - GetPosition();

	float3 fwd = direction.Normalized();
	float3 right = worldUp.Cross(fwd).Normalized();
	float3 up = fwd.Cross(right).Normalized();

	SetRotationAxis(right, up, fwd);
}
