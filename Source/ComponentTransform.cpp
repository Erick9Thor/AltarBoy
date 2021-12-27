#include "ComponentTransform.h"
#include "Component.h"
#include "GameObject.h"
#include "Globals.h"

ComponentTransform::ComponentTransform(GameObject* new_object, float3 position, Quat rotation, float3 scale) : Component(Component::Type::Transform, new_object),
	local_position(position), local_rotation(rotation), local_scale(scale)
{
	SetLocalTransform(position, rotation, scale);
}

ComponentTransform::ComponentTransform(GameObject* new_object, const float4x4& new_transform) :
	Component(Component::Type::Transform, new_object)
{
	SetLocalTransform(new_transform);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::LookAt(float3 target, float3 world_up)
{
	float3 direction = target - GetPosition();

	float3 fwd = direction.Normalized();
	float3 right = world_up.Cross(fwd).Normalized();
	float3 up = fwd.Cross(right).Normalized();

	SetRotationAxis(right, up, fwd);
}

void ComponentTransform::SetRotationAxis(float3 x, float3 y, float3 z)
{
	transform.SetCol3(0, x);
	transform.SetCol3(1, y);
	transform.SetCol3(2, z);

	SetGlobalTransform(transform);
}

inline void ComponentTransform::SetLocalTransform(float3 position, Quat rotation, float3 scale)
{
	float4x4 new_transform = float4x4::FromTRS(position, rotation, scale);
	SetLocalTransform(new_transform);
}

void ComponentTransform::SetLocalTransform(float4x4 new_transform)
{
	local_transform = new_transform;
	local_transform.Decompose(local_position, local_rotation, local_scale);
	local_rotation_euler = local_rotation.ToEulerXYZ() * to_deg;

	// TODO: Update hierachy of childs recursively (for each child recursively call these lines encapsulated in a method)
	if (game_object->parent) {
		transform = game_object->parent->GetComponent<ComponentTransform>()->GetTransform() * local_transform;
	}
	else {
		transform = local_transform;
	}
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	if (game_object->parent)
	{
		// Use parent global transform to get local transform
		SetLocalTransform(game_object->parent->GetComponent<ComponentTransform>()->GetTransform().Transposed() * transform);
	}
	else
	{
		SetLocalTransform(transform);
	}
}

void ComponentTransform::OnTransformUpdated()
{
}

void ComponentTransform::SetPosition(float3 new_transform)
{
	transform.SetTranslatePart(new_transform);
	SetGlobalTransform(transform);
}

void ComponentTransform::DrawGui()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}