#include "RenderList.h"

#include "GameObject.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentMesh.h"

void RenderList::Update(ComponentCamera* camera, GameObject* game_object)
{
	nodes.clear();
	CollectObjects(camera, game_object);
}

void RenderList::CollectObjects(ComponentCamera* camera, GameObject* game_object)
{
	Frustum frustum = camera->GetFrustum();
	// TODO: Debug this
	bool inside = frustum.Intersects(game_object->GetOBB());

	if (inside)
	{
		float3 camera_pos = frustum.WorldMatrix().TranslatePart();
		CollectMesh(camera_pos, game_object);
	}

	for (GameObject* child : game_object->childs)
		CollectObjects(camera, child);
}

void RenderList::CollectMesh(const float3& camera_pos, GameObject* game_object)
{
	ComponentMesh* mesh = game_object->GetComponent<ComponentMesh>();
	if (mesh && mesh->IsVisible())
	{
		RenderTarget target;
		target.name = game_object->name.c_str();
		target.game_object = game_object;
		target.mesh = mesh;
		target.distance = (game_object->GetOBB().CenterPoint() - camera_pos).LengthSq();

		// Get first element which distance is not less than current target one
		std::vector<RenderTarget>::iterator it = std::lower_bound(nodes.begin(), nodes.end(), target, 
			[](const RenderTarget& it_target, const RenderTarget& new_target)
			{
				return it_target.distance < new_target.distance;
			});
		nodes.insert(it, target);
	}
	
}
