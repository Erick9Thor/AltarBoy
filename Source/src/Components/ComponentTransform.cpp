#include "ComponentTransform.h"
#include "Component.h"

#include "../GameObject.h"
#include "../Globals.h"
#include <imgui.h>

ComponentTransform::ComponentTransform(GameObject* new_object, float3 position, Quat rotation, float3 scale)
	: Component(Component::Type::TRANSFORM, new_object)
	, local_position(position)
	, local_rotation(rotation)
	, local_scale(scale)
{
	SetLocalTransform(position, rotation, scale);
}

ComponentTransform::ComponentTransform(GameObject* new_object, const float4x4& new_transform)
	: Component(Component::Type::TRANSFORM, new_object)
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

inline void ComponentTransform::SetLocalPosition(float3 new_position)
{
	SetLocalTransform(new_position, local_rotation, local_scale);
}

inline void ComponentTransform::SetLocalScale(float3 new_scale)
{
	SetLocalTransform(local_position, local_rotation, new_scale);
}

inline void ComponentTransform::SetLocalRotation(Quat new_rotation)
{
	SetLocalTransform(local_position, new_rotation, local_scale);
}

void ComponentTransform::SetLocalRotation(float3 rotation_angles)
{
	float3 rotation = rotation_angles * to_rad;
	SetLocalTransform(local_position, Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z).Normalized(), local_scale);
}

void ComponentTransform::SetLocalTransform(float4x4 new_transform)
{
	local_transform = new_transform;
	local_transform.Decompose(local_position, local_rotation, local_scale);
	local_rotation_euler = RadToDeg(local_rotation.ToEulerXYZ());

	UpdateGlobalTransformHierarchy();
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	// Use parent global transform to get local transform
	if (game_object->parent)
		SetLocalTransform(game_object->parent->GetComponent<ComponentTransform>()->GetTransform().Transposed() * transform);
	else
		SetLocalTransform(transform);
}

void ComponentTransform::UpdateGlobalTransformHierarchy()
{
	// Updates current transform based on parent and calls to update the children
	if (game_object->parent)
		transform = game_object->parent->GetComponent<ComponentTransform>()->GetTransform() * local_transform;
	else
		transform = local_transform;

	// Set transform as changed
	changed = true;

	// Update children
	for (GameObject* child : game_object->childs)
		child->GetComponent<ComponentTransform>()->UpdateGlobalTransformHierarchy();
}

void ComponentTransform::SetPosition(float3 new_transform)
{
	transform.SetTranslatePart(new_transform);
	SetGlobalTransform(transform);
}

void ComponentTransform::Save(JsonFormaterValue j_component) const
{
	JsonFormaterValue j_position = j_component["Position"];

	j_position[0] = local_position.x;
	j_position[1] = local_position.y;
	j_position[2] = local_position.z;

	JsonFormaterValue j_rotation = j_component["Rotation"];

	j_rotation[0] = local_rotation.x;
	j_rotation[1] = local_rotation.y;
	j_rotation[2] = local_rotation.z;

	JsonFormaterValue j_scale = j_component["Scale"];

	j_scale[0] = local_scale.x;
	j_scale[1] = local_scale.y;
	j_scale[2] = local_scale.z;
}

void ComponentTransform::Load(JsonFormaterValue j_component)
{
	JsonFormaterValue j_position = j_component["Position"];
	JsonFormaterValue j_rotation = j_component["Rotation"];
	JsonFormaterValue j_scale = j_component["Scale"];

	SetLocalTransform(
		float3(j_position[0], j_position[1], j_position[2]),
		Quat(j_rotation[0], j_rotation[1], j_rotation[2], 0.0f),
		float3(j_scale[0], j_scale[1], j_scale[2]));
}

void ComponentTransform::DrawGui()
{
	static bool locked_scale = true;
	if (ImGui::CollapsingHeader("Local Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 position = local_position;
		float3 scale = local_scale;
		float3 rotation = local_rotation_euler;
		if (ImGui::InputFloat3("position", &position[0])) SetLocalPosition(position);
		if (ImGui::InputFloat3("scale", &scale[0])) SetLocalScale(scale);
		if (ImGui::InputFloat3("rotation", &rotation[0])) SetLocalRotation(rotation);
	}
}