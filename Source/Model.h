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
		void Load(const char* file_name);
		void Draw();

	private:
		void LoadMeshes(const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

		vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName);

		unsigned int TextureFromFile(const char* path, const string& directory);
		
		vector<unsigned> textures;
		vector<int> materials;
		vector<Mesh*> meshes;
		vector<Texture> textures_loaded;
		string directory;
};

