#include "Globals.h"
#include "Model.h"
#include "Application.h"

#include "ModuleTexture.h"

#include "glew.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include "il.h"
#include "ilu.h"

Model::Model(const char* file_name)
{
	LOG("[Model] Loading model from this path: %s", file_name);

	m_Name = file_name;

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality || aiProcess_Triangulate);
	if (scene)
	{
		LoadTextures(scene);
		LoadMeshes(scene);

		for (Mesh& mesh : meshes)
		{
			num_vertices += mesh.GetNumVertices();
			num_triangles += mesh.GetNumIndices() / 3;
		}
	}
	else
	{
		LOG("[Model] Error loading %s: %s", file_name, aiGetErrorString());
	}
}

Model::~Model()
{}

void Model::Draw()
{
	for (Mesh& mesh : meshes) {
		mesh.Draw(textures);
	}
}

void Model::LoadTextures(const aiScene* scene)
{
	LOG("[Model] Texture loading for model...");

	aiString file;
	textures.reserve(scene->mNumMaterials);
	
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		static const int index = 0;
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
		{
			Texture texture = App->texture->LoadTexture(file.data);
			textures.push_back(texture);
		}
		else {
			LOG("Failed loading texture %s:", aiGetErrorString());
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	LOG("[Model] Loading meshes");

	textures.reserve(scene->mNumMeshes);
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.push_back(Mesh(scene->mMeshes[i]));
	}
}

void Model::DrawGui()
{
	ImGui::Text("Name: %s", m_Name);
	ImGui::Text("Num vertices: %i", num_vertices);
	ImGui::Text("Num triangles: %i", num_triangles);
}

void Model::CleanUp()
{
	LOG("[Model] Cleaning model");

	for (Mesh& mesh : meshes) {
		mesh.CleanUp();
	}
	
}
