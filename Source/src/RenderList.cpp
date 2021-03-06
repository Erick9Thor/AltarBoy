#include "RenderList.h"

#include "GameObject.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentMesh.h"
#include "Quadtree.h"

void RenderList::Update(ComponentCamera* camera, GameObject* game_object)
{
	nodes.clear();
	Frustum* frustum = camera->GetFrustum();
	float3 camera_pos = frustum->WorldMatrix().TranslatePart();
	CollectObjects(camera, camera_pos, game_object);
}

void RenderList::Update(ComponentCamera* camera, QuadtreeNode* quadtree)
{
	nodes.clear();
	Frustum* frustum = camera->GetFrustum();
	float3 camera_pos = frustum->WorldMatrix().TranslatePart();
	CollectObjects(camera, camera_pos, quadtree);
}

void RenderList::CollectObjects(ComponentCamera* camera, const float3& camera_pos, GameObject* game_object)
{
	bool inside = camera->GetFrustum()->Intersects(game_object->GetOBB());
	if (inside) {
		
		CollectMesh(camera_pos, game_object);
	}

	for (GameObject* child : game_object->childs)
		CollectObjects(camera, camera_pos, child);
}

void RenderList::CollectObjects(ComponentCamera* camera, const float3& camera_pos, QuadtreeNode* quadtree)
{
	Frustum* frustum = camera->GetFrustum();
	bool quad_inside = frustum->Intersects(quadtree->GetBox());
	// Check if node intersects camera
	if (quad_inside) {
		// Check any gameobjects intersect
		for (GameObject* game_object : quadtree->GetObjects()) {
			bool object_inside = frustum->Intersects(game_object->GetOBB());

			if (object_inside) {
				CollectMesh(camera_pos, game_object);
			}
		}
		// Call for all childs (What to do if it is duplicated when collecting)?
		if (!quadtree->IsLeaf()) {
			for (QuadtreeNode* child : quadtree->childs)
			{
				CollectObjects(camera, camera_pos, child);
			}
		}	
	}
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
