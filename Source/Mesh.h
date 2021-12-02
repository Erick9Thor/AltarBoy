#pragma once

#include "ModuleTexture.h"

#include "assimp/scene.h"
#include <vector>


class Mesh
{
	public:
	
		Mesh(const aiMesh* mesh);
		~Mesh();

		void Draw(const std::vector<Texture*>& model_textures);

		unsigned int GetNumVertices() const {
			return num_vertices;
		};
		unsigned int GetNumIndices() const {
			return num_indices;
		};
	
	private:
		void LoadVBO(const aiMesh* mesh);
		void LoadEBO(const aiMesh* mesh);
		void CreateVAO();

		unsigned VBO, EBO, VAO;
		unsigned num_vertices, num_indices, texture_index;
};

