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



Model::Model(const string& model_path)
{

	path = model_path.substr(0, model_path.find_last_of("/\\") + 1);

	if (path.size() > 0) {
		LOG("[Model] Loading model from this path: %s", path.c_str());
	}

	file_name = model_path.substr(model_path.find_last_of("/\\") + 1);
	name = file_name.substr(0, std::string::size_type(file_name.find_last_of('.')));

	LOG("[Model] Loading model: %s", name.c_str());

	const aiScene* scene = aiImportFile(model_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality || aiProcess_Triangulate);
	if (scene)
	{
		LoadTextures(scene);
		LoadMeshes(scene);

		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			num_triangles += meshes[i]->GetNumIndices() / 3;
			num_vertices += meshes[i]->GetNumVertices();
		}
	}
	else
	{
		LOG("[Model] Error loading %s: %s", model_path, aiGetErrorString());
	}
}

Model::~Model()
{
	for (int i = 0; i < textures.size(); ++i)
		delete textures[i];
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
}

void Model::Draw()
{
	for (unsigned int i = 0; i < meshes.size(); ++i) {
		meshes[i]->Draw(textures);
	}
}

void Model::LoadTextures(const aiScene* scene)
{
	LOG("[Model] Texture loading for model...");

	aiString file;
	textures = vector<Texture*>(scene->mNumMaterials);
	
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			textures[i] = new Texture(file.data);
		}
		else {
			LOG("Failed loading texture %s:", aiGetErrorString());
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	LOG("[Model] Loading meshes");

	meshes = vector<Mesh*>(scene->mNumMeshes);

	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		meshes[i] = new Mesh(scene->mMeshes[i]);
	}
}

void Model::DrawGui()
{

}
