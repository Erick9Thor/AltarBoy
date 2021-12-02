#pragma once

#include "ModuleTexture.h"
#include "Texture.h"

#include "Mesh.h"
#include "assimp/scene.h"
#include <vector>


#include <string.h>
using namespace std;

class Model
{
	public:
		Model(const std::string& model_path);
		~Model();
		void Draw();
		void DrawGui();

	private:
		void LoadTextures(const aiScene* scene);
		void LoadMeshes(const aiScene* scene);

		vector<Texture*> textures;
		vector<Mesh*> meshes;

		unsigned int num_vertices = 0;
		unsigned int num_triangles = 0;
		
		string name;
		string path;
		string file_name;
};

