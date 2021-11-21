#pragma once

#include "assimp/scene.h"
#include <vector>

using namespace std;

class Mesh
{
	public:
	
		Mesh(const aiMesh* mesh);
	
		~Mesh();
	
		void LoadVBO(const aiMesh* mesh);
		void LoadEBO(const aiMesh* mesh);
		void CreateVAO();

		void Draw(const std::vector<unsigned>& model_textures);


		unsigned int GetNumVertices() const {
			return num_vertices;
		};
		unsigned int GetNumIndices() const {
			return num_indices;
		};
	
	private:
		unsigned VBO, EBO, VAO;
		unsigned num_vertices, num_indices, texture_index;
};

