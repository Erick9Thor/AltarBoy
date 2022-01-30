#include "ModelImporter.h"

#include "../Utils/Logger.h"

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

ResourceModel* ModelImporter::ImportModel(const char* file_path)
{
	ResourceModel* resource = nullptr;

	// Parse path?¿?¿?

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_GlobalScale);

	if (scene)
	{
		LoadAssimpNode(scene, scene->mRootNode);
	}
	else
	{
		// LOG("Error loading file %s: %s", model_path.c_str(), aiGetErrorString());
	}

	importer.FreeScene();


	return resource;
}

void ModelImporter::LoadAssimpNode(const aiScene* scene, const aiNode* node)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		/*GameObject* model_part = CreateNewGameObject(node->mName.C_Str(), parent);
		model_part->CreateComponent(Component::Type::MESH);
		model_part->GetComponent<ComponentMesh>()->Import(mesh);
		model_part->CreateComponent(Component::Type::MATERIAL);
		model_part->GetComponent<ComponentMaterial>()->Import(scene->mMaterials[mesh->mMaterialIndex], model_path, node->mName.C_Str());

		aiVector3D aiTranslation, aiScale;
		aiQuaternion aiRotation;
		node->mTransformation.Decompose(aiScale, aiRotation, aiTranslation);
		model_part->GetComponent<ComponentTransform>()->SetLocalTransform(
			float3(aiTranslation.x, aiTranslation.y, aiTranslation.z),
			Quat(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w),
			float3(aiScale.x, aiScale.y, aiScale.z));*/
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadAssimpNode(scene, node->mChildren[i]);
	}
}
