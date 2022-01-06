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
	ComponentMesh(GameObject* conatiner);
	~ComponentMesh() override;
	static inline Type GetType()
	{
		return Type::Mesh;
	};

	void Load(const aiMesh* mesh);
	void Draw(ComponentCamera* camera) override;
	void CleanUp();

	inline unsigned GetNumVertices() const
	{
		return num_vertices;
	}
	inline unsigned GetNumIndexes() const
	{
		return num_indices;
	}
	AABB GetAABB() const
	{
		return bounding_box;
	}
	bool IsLoaded() const
	{
		return loaded;
	}
	void DrawGui() override;

private:
	void Import(const aiMesh* mesh);
	void GenerateBuffers();
	void GenerateAABB();

	AABB bounding_box;
	bool loaded;

	unsigned num_indices;
	unsigned num_vertices;

	// TODO: Improve structure
	unsigned vbo, ebo;
	// Store the ids of buffers in opengl
	unsigned vao;
	unsigned indices_buffer;
	unsigned vertices_buffer;
	unsigned normals_buffer;
	unsigned tex_coords_buffer;

	// Track buffer sizes
	unsigned indices_buffer_size;
	unsigned vertices_buffer_size;
	unsigned normals_buffer_size;
	unsigned tex_coords_buffer_size;

	// In memory buffers
	unsigned* indices;
	float* vertices;
	float* normals;
	float* tex_coords;
};
