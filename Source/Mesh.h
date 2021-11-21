#pragma once

#include <assimp/mesh.h>
#include <vector>
#include <MathGeoLib.h>
#include <string.h>

using namespace std;

struct Vertex {
	// position
	float3 Position;
	// normal
	float3 Normal;
	// texCoords
	float2 TexCoords;

	float3 Tangent;
	// bitangent
	float3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[4];
	//weights from each bone
	float m_Weights[4];
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
	public:
		Mesh(aiMesh* mesh, vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		void Draw(unsigned int indx);

	private:
		void CreateVAO();
		void LoadVBO(const aiMesh* mesh);
		void LoadEBO(const aiMesh* mesh);

		unsigned int VAO, VBO, EBO;
		unsigned int num_vertices, num_indices;
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
};

