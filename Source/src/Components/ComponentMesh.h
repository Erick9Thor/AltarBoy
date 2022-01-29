#pragma once
#include "Component.h"
#include "../Modules/ModuleTexture.h"

#include "../Resources/ResourceMesh.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>

class GameObject;
class ComponentCamera;
class Program;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* conatiner);
	~ComponentMesh() override;

	void Draw(ComponentCamera* camera, Program* program) override;

	static inline Type GetType() { return Type::MESH; };
	
	void CleanUp();

	inline bool IsLoaded() const { return resource->loaded; }
	bool IsVisible() const { return visible; }

	AABB GetAABB() const { return resource->bounding_box; }
	inline unsigned GetBufferSize(ResourceMesh::Buffers buffer) const { return resource->buffer_sizes[buffer]; }
	inline unsigned GetBufferId(ResourceMesh::Buffers buffer) const { return resource->buffer_ids[buffer]; }
	const float* GetVertices() const { return resource->vertices; }
	const unsigned* GetIndices() const { return resource->indices; }

	void Import(const aiMesh* mesh);
	
	void Save(JsonFormaterValue j_component) const override;
	void Load(JsonFormaterValue j_component) override;

	void DrawGui() override;

private:
	bool visible = true;
	ResourceMesh* resource = nullptr;
};
