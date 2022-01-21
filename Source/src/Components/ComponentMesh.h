#pragma once
#include "Component.h"
#include "../Modules/ModuleTexture.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>

class GameObject;
class ComponentCamera;

class ComponentMesh : public Component
{
public:

	enum Buffers
	{
		b_indices = 0,
		b_vertices,
		b_normals,
		b_tex_coords,
		n_buffers,
	};

	ComponentMesh(GameObject* conatiner);
	~ComponentMesh() override;
	static inline Type GetType() { return Type::Mesh; };

	void Load(const aiMesh* mesh);
	void Draw(ComponentCamera* camera) override;
	void CleanUp();

	inline bool IsLoaded() const { return loaded; }
	bool IsVisible() const { return visible; }

	AABB GetAABB() const { return bounding_box; }
	inline unsigned GetBufferSize(Buffers buffer) const { return buffer_sizes[buffer]; }
	inline unsigned GetBufferId(Buffers buffer) const { return buffer_ids[buffer]; }
	const float* GetVertices() const { return vertices; }
	const unsigned* GetIndices() const { return indices; }
	

	void DrawGui() override;

private:
	void Import(const aiMesh* mesh);
	void GenerateBuffers();
	void GenerateAABB();

	AABB bounding_box;
	bool loaded = false;
	bool visible = true;

	// Store the ids of buffers in opengl
	unsigned vbo, ebo;
	unsigned vao;
	unsigned buffer_ids[n_buffers];

	// Track buffer sizes
	unsigned buffer_sizes[n_buffers];

	// In memory buffers
	unsigned* indices;
	float* vertices;
	float* normals;
	float* tex_coords;
};
