#include "Globals.h"

#include "Utils/Logger.h"

#include "Application.h"
#include "Modules/ModuleDebugDraw.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/ComponentTransform.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentMesh.h"
#include "Components/ComponentMaterial.h"
#include "Modules/ModuleTexture.h"
#include "Modules/ModuleProgram.h"

#include "Skybox.h"

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

Scene::Scene()
{
	root = new GameObject(nullptr, float4x4::identity, "Root");
	skybox = new Skybox();
	//GameObject* test_model = LoadFBX("BakerHouse.fbx"); // Need to call when opengl is running
}

Scene::~Scene()
{
	delete root;
	delete skybox;
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
		std::vector<Texture> textures = LoadTextures(scene, model_path);
		model = CreateNewGameObject(name.c_str(), root);
		LoadNode(scene, scene->mRootNode, model, textures);
		textures.clear();
	}
	else
	{
		LOG("Error loading file %s: %s", file_name.c_str(), aiGetErrorString());
	}
	importer.FreeScene();
	model->OnTransformUpdated();
	return model;
}

void Scene::LoadNode(const aiScene* scene, const aiNode* node, GameObject* parent, std::vector<Texture>& textures)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		GameObject* model_part = CreateNewGameObject(node->mName.C_Str(), parent);
		model_part->CreateComponent(Component::Type::Mesh);
		model_part->GetComponent<ComponentMesh>()->Load(mesh);
		model_part->CreateComponent(Component::Type::Material);
		model_part->GetComponent<ComponentMaterial>()->SetTexture(textures[mesh->mMaterialIndex]);

		aiVector3D aiTranslation, aiScale;
		aiQuaternion aiRotation;
		node->mTransformation.Decompose(aiScale, aiRotation, aiTranslation);
		model_part->GetComponent<ComponentTransform>()->SetLocalTransform(
			float3(aiTranslation.x, aiTranslation.y, aiTranslation.z),
			Quat(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w),
			float3(aiScale.x, aiScale.y, aiScale.z));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(scene, node->mChildren[i], parent, textures);
	}
}

std::vector<Texture> Scene::LoadTextures(const aiScene* scene, const std::string& model_path)
{
	std::vector<Texture> textures;
	textures.reserve(scene->mNumMaterials);
	LOG("Loading %d textures", scene->mNumMaterials)
	for (unsigned i = 0; i < scene->mNumMaterials; i++)
	{
		Texture texture = LoadTexture(scene->mMaterials[i], model_path);
		textures.push_back(texture);
	}
	return textures;
}

Texture Scene::LoadTexture(const aiMaterial* material, const std::string& model_path)
{
	static const int index = 0;
	aiString file;
	Texture texture;
	texture.loaded = false;
	if (material->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
	{
		std::string model_texture_path(file.data);
		std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
		std::string default_path("Textures\\");

		texture = App->texture->Load(file.data);
		if (texture.loaded)
			return texture;

		texture = App->texture->Load((model_path + texture_file).c_str());
		if (texture.loaded)
			return texture;

		texture = App->texture->Load((default_path + texture_file).c_str());
		if (!texture.loaded)
			LOG("Error loading textures: %s", aiGetErrorString());
	}
	return texture;
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
	root->Update();
}

void Scene::Draw(ComponentCamera* camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, camera->GetFrameBuffer());

	unsigned res_x, res_y;
	camera->GetResolution(res_x, res_y);
	glViewport(0, 0, res_x, res_y);

	// TODO: Change with skybox
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	if (skybox_active) {
		skybox->Draw(camera);
	}
	
	float4x4 view = camera->GetViewMatrix(false);
	float4x4 proj = camera->GetProjectionMatrix(false);

	App->debug_draw->Draw(view, proj, res_x, res_y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	root->DrawAll(camera);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::DebugDraw()
{
	root->DebugDrawAll();
}

void Scene::OptionsMenu() {
	static bool bounding_boxes = false;
	ImGui::Checkbox("Skybox", &skybox_active);
	if (ImGui::Checkbox("Draw Bounding Boxes", &bounding_boxes))
		App->debug_draw->ToggleBoundingBoxes(bounding_boxes);
}