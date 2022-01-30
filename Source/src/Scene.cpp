#include "Scene.h"

#include "Globals.h"

#include "Utils/Logger.h"

#include "Application.h"
#include "Modules/ModuleDebugDraw.h"

#include "GameObject.h"
#include "Components/Component.h"
#include "Components/ComponentTransform.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentMesh.h"
#include "Components/ComponentMaterial.h"
#include "Components/ComponentPointLight.h"
#include "Components/ComponentSpotLight.h"

#include "Modules/ModuleTexture.h"
#include "Modules/ModuleProgram.h"
#include "Modules/ModuleEditor.h"

#include "Resources/Resource.h"
#include "Importers/SceneImporter.h"

#include "Skybox.h"
#include "Quadtree.h"

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include <map>

Scene::Scene()
{
	quadtree = new Quadtree();
	skybox = new Skybox();

	quadtree->SetBox(AABB(float3(-500, 0, -500), float3(500, 250, 500)));
	root = new GameObject(nullptr, float4x4::identity, "Root");
		
	//CreateDebugCamera();
	//CreateLights();
}

Scene::~Scene()
{
	CleanScene();
}

void Scene::CleanScene()
{
	App->editor->SetSelectedGO(nullptr);
	delete root;
	delete skybox;
	delete quadtree;
}

void Scene::DestroyGameObject(GameObject* game_object)
{
	if (App->editor->GetSelectedGO() == game_object)
		App->editor->SetSelectedGO(nullptr);
	quadtree->Remove(game_object);
}

void Scene::AddGameObject(GameObject* new_object, GameObject* parent)
{
	GameObject* new_parent = parent ? parent : root;
	new_parent->childs.push_back(new_object);
	quadtree->Insert(new_object);
}

GameObject* Scene::CreateNewGameObject(const char* name, GameObject* parent)
{
	// It will insert itself into quadtree on first bounding box update
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
		//std::vector<ResourceMaterial*> materials = LoadMaterials(scene, model_path, name);
		model = CreateNewGameObject(name.c_str(), root);
		LoadNode(scene, scene->mRootNode, model, model_path);
		//materials.clear();
	}
	else
	{
		LOG("Error loading file %s: %s", model_path.c_str(), aiGetErrorString());
	}
	importer.FreeScene();
	return model;
}

void Scene::Save(JsonFormaterValue j_scene) const
{
	// GameObjects
	JsonFormaterValue j_root = j_scene["GORoot"];
	root->Save(j_root);

	//TODO: Save camera position
}

void Scene::Load(JsonFormaterValue j_scene)
{
	//CleanScene(); 
	delete root;

	// Load GameObjects
	JsonFormaterValue j_root = j_scene["GORoot"];
	root = new GameObject(nullptr, "Root", j_root["Uid"]);
	root->scene_owner = this;
	root->Load(j_root);
	//CreateDebugCamera();
}

GameObject* Scene::RayCast(const LineSegment& segment) const
{
	GameObject* selected = nullptr;
	float closest_hit_distance = inf;

	map<float, GameObject*> game_objects;
	quadtree->GetRoot()->GetIntersections(game_objects, segment);

	for (auto game_object_pair: game_objects)
	{
		GameObject* game_object = game_object_pair.second;
		ComponentMesh* mesh = game_object->GetComponent<ComponentMesh>();
		if (mesh)
		{	
			// Transform ray to mesh space, more efficient
			LineSegment local_segment(segment);
			local_segment.Transform(game_object->GetComponent<ComponentTransform>()->GetTransform().Inverted());
			
			const float* vertices = mesh->GetVertices();
			const unsigned* indices = mesh->GetIndices();
			for (unsigned i = 0; i < mesh->GetBufferSize(ResourceMesh::Buffers::b_indices); i += 3)
			{
				Triangle triangle;
				triangle.a = vec(&vertices[indices[i] * 3]);
				triangle.b = vec(&vertices[indices[i + 1] * 3]);
				triangle.c = vec(&vertices[indices[i + 2] * 3]);

				float hit_distance;
				float3 hit_point;
				if (local_segment.Intersects(triangle, &hit_distance, &hit_point))
				{
					if (hit_distance < closest_hit_distance)
					{
						closest_hit_distance = hit_distance;
						selected = game_object;
					}
				}
				
			}
		}
	}
	return selected;
}

void Scene::CreateLights()
{
	GameObject* sun = CreateNewGameObject("Sun", root);
	sun->GetComponent<ComponentTransform>()->SetLocalPosition(float3(1, 1, -1));
	sun->GetComponent<ComponentTransform>()->LookAt(float3(0, 0, 0));
	sun->CreateComponent(Component::Type::DIRLIGHT);

	GameObject* spot = CreateNewGameObject("Spot Light", root);
	spot->GetComponent<ComponentTransform>()->SetLocalPosition(float3(-1, 1, -1));

	spot->CreateComponent(Component::Type::SPOTLIGHT);

	GameObject* point = CreateNewGameObject("Point Light", root);
	point->GetComponent<ComponentTransform>()->SetLocalPosition(float3(0, 1, -1));
	point->CreateComponent(Component::Type::POINTLIGHT);
}

void Scene::LoadNode(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& model_path)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		GameObject* model_part = CreateNewGameObject(node->mName.C_Str(), parent);
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
			float3(aiScale.x, aiScale.y, aiScale.z));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(scene, node->mChildren[i], parent, model_path);
	}
}

GameObject* Scene::CreateDebugCamera()
{
	GameObject* camera = CreateNewGameObject("Debug Camera", root);
	camera->GetComponent<ComponentTransform>()->SetLocalPosition(float3(5, 5, 0));
	camera->CreateComponent(Component::Type::CAMERA);
	camera->GetComponent<ComponentTransform>()->LookAt(float3(0, 5, 0));

	ComponentCamera* debug_camera = camera->GetComponent<ComponentCamera>();
	debug_camera->SetFarPlane(100.0f);
	debug_camera->draw_frustum = true;

	return camera;
}

void Scene::Update()
{
	root->Update();
}