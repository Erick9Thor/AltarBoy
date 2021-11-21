#pragma once

#include "Globals.h"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

#include <vector>
#include <glew.h>
#include <string>
using namespace std;

class Model
{
	public:
		Model(const char* file_name);
		void Draw();

	private:
		void LoadMeshes(const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

		vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName);

		unsigned int TextureFromFile(const char* path);
		
		vector<Texture> textures_loaded;
		vector<int> materials;
		vector<Mesh*> meshes;
		string directory;
};

