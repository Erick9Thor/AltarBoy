#include "ComponentTransform.h"
#include "Component.h"
#include "GameObject.h"
#include "Globals.h"

ComponentTransform::ComponentTransform(GameObject* new_GameObject, const float4x4& transform) :
	Component(Component::Type::Transform, false)
{
	local_transform = transform;
	local_transform.Decompose(local_position, local_rotation, local_scale);
	local_rotation_euler = local_rotation.ToEulerXYZ() * RADTODEG;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::LookAt(float3 target, float3 worldUp)
{
}
