#include "ComponentTransform.h"
#include "Component.h"

#include "../GameObject.h"
#include "../Globals.h"

ComponentTransform::ComponentTransform(GameObject* new_object, float3 position, Quat rotation, float3 scale)
	: Component(Component::Type::Transform, new_object)
	, local_position(position)
	, local_rotation(rotation)
	, local_scale(scale)
{
	SetLocalTransform(position, rotation, scale);
}

ComponentTransform::ComponentTransform(GameObject* new_object, const float4x4& new_transform)
	: Component(Component::Type::Transform, new_object)
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

void ComponentTransform::SetLocalRotation(float3 rotation_angles)
{
	float3 rotation = rotation_angles * to_rad;
	SetLocalTransform(local_position, Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z).Normalized(), local_scale);
}

void ComponentTransform::SetLocalTransform(float4x4 new_transform)
{
	local_transform = new_transform;
	local_transform.Decompose(local_position, local_rotation, local_scale);
	local_rotation_euler = local_rotation.ToEulerXYZ() * to_deg;

	UpdateGlobalTransforms();
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	// Use parent global transform to get local transform
	if (game_object->parent)
		SetLocalTransform(game_object->parent->GetComponent<ComponentTransform>()->GetTransform().Transposed() * transform);
	else
		SetLocalTransform(transform);
}

void ComponentTransform::UpdateGlobalTransforms()
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
		child->GetComponent<ComponentTransform>()->UpdateGlobalTransforms();
}

void ComponentTransform::SetPosition(float3 new_transform)
{
	transform.SetTranslatePart(new_transform);
	SetGlobalTransform(transform);
}

void ComponentTransform::DrawGui()
{
	static bool locked_scale = true;
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 position = local_position;
		float3 scale = local_scale;
		float3 rotation = local_rotation_euler;

		ImGui::Text("Translation");
		if (ImGui::SliderFloat3("t.XYZ", &position[0], -5.0f, 5.0f))
			SetLocalPosition(position);

		ImGui::Separator();
		ImGui::Text("Scale");
		ImGui::Checkbox("Lock", &locked_scale);
		ImGui::SameLine();

		float3 scale_delta = scale;
		if (ImGui::SliderFloat3("s.XYZ", &scale[0], 0.005f, 5.0f))
		{
			if (locked_scale)
			{
				scale_delta -= scale;
				for (int i = 0; i < 3; i++)
				{
					if (scale_delta[i] != 0.0f)
					{
						scale = float3(scale[i]);
						break; // Only one axis can change
					}
				}
			}
			SetLocalScale(scale);
		}

		ImGui::Separator();
		ImGui::Text("Rotation");
		if (ImGui::SliderFloat3("r.XYZ", &rotation[0], 0.0f, 360.0f))
			SetLocalRotation(rotation);
	}
}