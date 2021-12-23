#pragma once
#include "Component.h"
#include "ModuleTexture.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>

class GameObject;

class ComponentMesh :
    public Component
{
public:
    ComponentMesh(GameObject* conatiner);
    ~ComponentMesh() override;
	static inline Type GetType() { return Type::Mesh; };

	void Load(const aiMesh* mesh);
	void Draw();
	void CleanUp();

	inline unsigned GetNumVertices() const { return num_vertices; }
	inline unsigned GetNumIndexes() const { return num_indices; }
	const AABB& GetAABB() const { return bounding_box; }
	bool IsLoaded() const { return loaded; }
	void DrawGui() override;

private:
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void GenerateAABB(const aiMesh* mesh);

	struct Vertex
	{
		float3 position;
		float3 normal;
		float2 tex_coords;
	};

	AABB bounding_box;

	bool loaded;
	unsigned vbo;
	unsigned num_vertices;
	unsigned ebo;
	unsigned num_indices;
	unsigned vao;
};
