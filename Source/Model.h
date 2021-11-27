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
		void DrawGui();

		void CleanUp();

	private:
		void LoadTextures(const aiScene* scene);
		void LoadMeshes(const aiScene* scene);
		Texture LoadTexture(const char* path);
		vector<unsigned> textures;
		vector<Mesh> meshes;

		unsigned int num_vertices = 0;
		unsigned int num_triangles = 0;
		const char* m_Name;
};

