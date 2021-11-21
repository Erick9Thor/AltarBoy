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
	
	private:
		unsigned VBO, EBO, VAO;
		unsigned num_vertices, num_indices, texture_index;
};

