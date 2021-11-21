#pragma once

#include "Mesh.h"

#include "assimp/scene.h"

#include <vector>
using namespace std;

struct Texture {
	unsigned id;	
	string path;
};

class Model
{
	public:
		Model(const char* file_name);
		~Model();
		void Draw();

	private:
		void LoadTextures(const aiScene* scene);
		void LoadMeshes(const aiScene* scene);
		Texture LoadTexture(const char* path);
		vector<unsigned> textures;
		vector<Mesh> meshes;
};

