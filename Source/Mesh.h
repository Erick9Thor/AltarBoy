#pragma once

#include "ModuleTexture.h"

#include "assimp/scene.h"
#include <vector>
#include "Math.h"


class Mesh
{
	public:
	
		Mesh(const aiMesh* mesh);
		~Mesh();

		void Draw(const std::vector<Texture*>& model_textures, float4x4& model);

		unsigned int GetNumVertices() const {
			return num_vertices;
		};
		unsigned int GetNumIndices() const {
			return num_indices;
		};

		AABB getAABB() {
			return aabb;
		}

		void DrawImGui();
	
	private:
		const aiMesh* mesh;

		void CreateAABB();
		void LoadVBO();
		void LoadEBO();
		void CreateVAO();

		unsigned VBO, EBO, VAO;
		unsigned num_vertices, num_indices, texture_index;

		AABB aabb;
};

