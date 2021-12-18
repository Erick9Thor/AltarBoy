#include "Globals.h"

#include "Application.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleTexture.h"

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

Scene::Scene()
{
	root = new GameObject(nullptr, float4x4::identity, "Root");
	//GameObject* test_model = LoadFBX("BakerHouse.fbx"); // Need to call when opengl is running
}

Scene::~Scene()
{
	delete root;
	delete main_camera;
}

void Scene::AddGameObject(GameObject* new_object, GameObject* parent)
{

}

GameObject* Scene::CreateNewGameObject(const char* name, GameObject* parent)
{
	GameObject* foo = new GameObject(parent ? parent : root, name);
	foo->scene_owner = this;
	return foo;
}


GameObject* Scene::LoadFBX(const std::string& path)
{
	// TODO: Set name from filename

	auto model_path = path.substr(0, path.find_last_of("/\\") + 1);
	auto file_name = path.substr(path.find_last_of("/\\") + 1);
	auto name = file_name.substr(0, std::string::size_type(file_name.find_last_of('.')));
	GameObject* model = nullptr;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (scene)
	{
		bool success;
		success = LoadTextures(scene, model_path);
		if (success) {
			model = CreateNewGameObject("TestModel", root);
			LoadNode(scene, scene->mRootNode, model);
		}
		else {
			LOG("Could not load model %s",file_name.c_str())
			// TODO: Clean if it fails
		}
	}
	else
	{
		LOG("Error loading file %s: %s", file_name.c_str(), aiGetErrorString());
	}
	importer.FreeScene();	

	return model;
}

void Scene::LoadNode(const aiScene* scene, const aiNode* node, GameObject* parent)
{
	GameObject* node_object = CreateNewGameObject("TestModelNode", parent);

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		GameObject* model_part = CreateNewGameObject("TestModelPart", node_object);
		model_part->CreateComponent(Component::Type::Mesh);
		model_part->GetComponent<ComponentMesh>()->Load(mesh);
		model_part->CreateComponent(Component::Type::Material);
		model_part->GetComponent<ComponentMaterial>()->SetTexture(textures[mesh->mMaterialIndex]);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(scene, node->mChildren[i], node_object);
	}
}

bool Scene::LoadTextures(const aiScene* scene, const std::string& model_path)
{
	textures.reserve(scene->mNumMaterials);
	LOG("Loading %d textures", scene->mNumMaterials)
		for (unsigned i = 0; i < scene->mNumMaterials; i++)
		{
			// Atm we only support loading a single texture so index is hardcoded to 0
			if (!LoadTexture(scene->mMaterials[i], model_path))
				return false;
		}
	return true;
}

bool Scene::LoadTexture(const aiMaterial* texture, const std::string& model_path)
{
	static const int index = 0;
	aiString file;
	if (texture->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
	{
		std::string model_texture_path(file.data);
		std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
		std::string default_path("Textures\\");

		Texture texture;
		texture = App->texture->Load(file.data);
		if (!texture.loaded) {
			LOG("Failed to load texture from model params: %s", texture.path.c_str())
				texture = App->texture->Load((model_path + texture_file).c_str());
		}
		if (!texture.loaded)
		{
			LOG("Failed to load texture from model file path: %s", texture.path.c_str())
				texture = App->texture->Load((default_path + texture_file).c_str());
		}
		if (!texture.loaded) {
			LOG("Failed to load texture from default: %s", texture.path.c_str())
				LOG("Could not find texture %s")
				return false;
		}
		textures.push_back(texture);
		LOG("Loaded texture from %s", texture.path.c_str())
	}
	else {
		LOG("Error loading textures: %s", aiGetErrorString());
		return false;
	}
	return true;
}

GameObject* Scene::CreateCamera()
{
	GameObject* camera = CreateNewGameObject("Camera", root);
	camera->GetComponent<ComponentTransform>()->SetLocalPosition(float3(10, 10, 0));
	camera->CreateComponent(Component::Type::Camera);
	camera->GetComponent<ComponentTransform>()->LookAt(float3(0, 5, 0));

	main_camera = camera->GetComponent<ComponentCamera>();

	return camera;
}

const ComponentCamera* Scene::GetMainCamera() const
{
	return main_camera;
}

void Scene::Play()
{
}

void Scene::Stop()
{
}

void Scene::Update()
{
	// root->Update();
}

void Scene::Draw()
{
	root->DrawAll();
}
