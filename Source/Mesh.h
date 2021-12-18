#pragma once

#include "ModuleTexture.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>


class Mesh
{
public:

	Mesh();
	~Mesh();

	void Load(const aiMesh* mesh);
	void Draw(float4x4& model, const std::vector<Texture>& model_textures);
	void CleanUp();

	inline unsigned GetNumVertices() const { return num_vertices; }
	inline unsigned GetNumIndexes() const { return num_indices; }
	const AABB& GetAABB() const { return bounding_box; }
	bool IsLoaded() const { return loaded; }

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
	unsigned texture_index;
};

