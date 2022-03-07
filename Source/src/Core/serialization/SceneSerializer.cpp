#include "core/hepch.h"
#include "SceneSerializer.h"

#include "components/ComponentCamera.h"
#include "components/ComponentDirLight.h"
#include "components/ComponentMaterial.h"
#include "components/ComponentMesh.h"
#include "components/ComponentPointLight.h"
#include "components/ComponentSpotLight.h"
#include "components/ComponentTransform.h"

#include "modules/ModuleImporter.h"

Hachiko::Scene* Hachiko::SceneSerializer::Load(const char* path)
{
    if (!std::filesystem::exists(path))
    {
        return nullptr;
    }

    YAML::Node sceneNode = YAML::LoadFile(path);

    // Validate scene header
    if (!sceneNode[SCENE_NAME].IsDefined())
    {
        return nullptr;
    }
    preferences = dynamic_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));
    const auto sceneOutput = new Scene();
    //sceneOutput->SetSceneId(sceneNode[SCENE_ID].as<std::string>()); // Scenes don't have ID yet

    if (!sceneNode[CHILD_NODE].IsDefined())
    {
        return sceneOutput;
    }

    for (size_t i = 0; i < sceneNode[CHILD_NODE].size(); ++i)
    {
        LoadEntity(sceneNode[CHILD_NODE][i], sceneOutput);
    }
    return sceneOutput;
}

bool Hachiko::SceneSerializer::Save(const Scene* scene)
{
    YAML::Node scene_data;
    //scene_data[SCENE_ID] = scene->GetSceneId(); // Scenes don't have ID yet
    scene_data[SCENE_NAME] = scene->GetName();

    for (int i = 0; i < scene->GetRoot()->children.size(); ++i)
    {
        scene_data[CHILD_NODE][i] = SaveEntity(scene->GetRoot()->children[i]);
    }

    ResourcesPreferences* resources_preferences = static_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));
    std::ofstream fout(StringUtils::Concat(resources_preferences->GetAssetsPath(Resource::Type::SCENE), scene->GetName()));
    fout << scene_data;
    return true;
}

YAML::Node Hachiko::SceneSerializer::SaveEntity(const GameObject* game_object)
{
    YAML::Node save_scene;
    save_scene[ENTITY_NAME] = game_object->GetName();
    save_scene[ENTITY_ENABLED] = game_object->IsActive();
    save_scene[ENTITY_ID] = game_object->GetID();

    for (int i = 0; i < game_object->GetComponents().size(); ++i)
    {
        //save_scene[COMPONENT_NODE][i][COMPONENT_NAME] = std::string(game_object->GetComponents()[i]->GetName());
        save_scene[COMPONENT_NODE][i][COMPONENT_ENABLED] = game_object->GetComponents()[i]->IsActive();
        save_scene[COMPONENT_NODE][i][COMPONENT_ID] = game_object->GetComponents()[i]->GetID();
        save_scene[COMPONENT_NODE][i][COMPONENT_TYPE] = static_cast<unsigned int>(game_object->GetComponents()[i]->GetType());
        save_scene[COMPONENT_NODE][i][COMPONENT_DATA] = SaveComponent(game_object->GetComponents()[i]);
    }

    for (int i = 0; i < game_object->children.size(); ++i)
    {
        save_scene[CHILD_NODE][i] = SaveEntity(game_object->children[i]);
    }

    return save_scene;
}

YAML::Node Hachiko::SceneSerializer::SaveComponent(const Component* component) const
{
    YAML::Node component_node;

    switch (component->GetType())
    {
    case Component::Type::TRANSFORM:
        {
            const ComponentTransform* tc = static_cast<const ComponentTransform*>(component);

            component_node[POSITION_NODE] = tc->GetPosition();
            component_node[ROTATION_NODE] = tc->GetRotation();
            component_node[SCALE_NODE] = tc->GetScale();

            break;
        }

    case Component::Type::MESH:
        {
            const ComponentMesh* mc = static_cast<const ComponentMesh*>(component);

            component_node[MODEL_PATH] = mc->GetResourcePath();

            for (int i = 0; i < mc->GetMeshesCount(); ++i)
            {
                //component_node[MESH_NODE][i][MESH_ENABLED] = mc->IsMeshEnabled(i);

                //if (mc->IsMeshTextureLoaded(i))
                //{
                //    component_node[MESH_NODE][i][MESH_TEXTURE] = mc->GetMeshTextureName(i);
                //    component_node[MESH_NODE][i][MESH_TEXTURE_TYPE] = mc->GetMeshTextureType(i);
                //}
            }

            break;
        }

    case Component::Type::SPOTLIGHT:
        break;

    case Component::Type::POINTLIGHT:
        break;

    case Component::Type::DIRLIGHT:
        break;

    case Component::Type::CAMERA:
        break;

    default:
        break;
    }

    return component_node;
}

const Hachiko::GameObject* Hachiko::SceneSerializer::LoadEntity(YAML::Node gameObject_node, Scene* scene, GameObject* parent)
{
    // TODO: Entities could be load async
    GameObject* game_object = scene->CreateNewGameObject(parent, gameObject_node[ENTITY_NAME].as<std::string>().c_str());
    game_object->SetID(gameObject_node[ENTITY_ID].as<size_t>());

    if (gameObject_node[COMPONENT_NODE].IsDefined())
    {
        for (size_t i = 0; i < gameObject_node[COMPONENT_NODE].size(); ++i)
        {
            LoadComponent(gameObject_node[COMPONENT_NODE][i], game_object);
        }
    }

    if (gameObject_node[CHILD_NODE].IsDefined())
    {
        for (size_t i = 0; i < gameObject_node[CHILD_NODE].size(); ++i)
        {
            LoadEntity(gameObject_node[CHILD_NODE][i], scene, game_object);
        }
    }

    return nullptr;
}

void Hachiko::SceneSerializer::LoadComponent(YAML::Node component, GameObject* game_object) const
{
    switch (static_cast<Component::Type>(component[COMPONENT_TYPE].as<int>()))
    {
    case Component::Type::TRANSFORM:
        {
            ComponentTransform* component_transform = static_cast<ComponentTransform*>(game_object->CreateComponent(Component::Type::TRANSFORM));
            component_transform->SetID(component[COMPONENT_ID].as<size_t>());
            component[COMPONENT_ENABLED].as<bool>() ? component_transform->Enable() : component_transform->Disable();
            component_transform->SetPosition(component[COMPONENT_DATA][POSITION_NODE].as<float3>());
            component_transform->SetLocalRotation(component[COMPONENT_DATA][ROTATION_NODE].as<Quat>());
            component_transform->SetLocalScale(component[COMPONENT_DATA][SCALE_NODE].as<float3>());
            break;
        }
    case Component::Type::MESH:
        {
            ComponentMesh* component_mesh = static_cast<ComponentMesh*>(game_object->CreateComponent(Component::Type::MESH));
            component_mesh->SetID(component[COMPONENT_ID].as<size_t>());
            component[COMPONENT_ENABLED].as<bool>() ? game_object->GetComponent<ComponentMesh>()->Enable() : game_object->GetComponent<ComponentMesh>()->Disable();

            // if not present in library import from assets folder
            if (!CheckIfImported(component[COMPONENT_DATA][MODEL_PATH].as<std::string>().c_str()))
            {
                ImportFromAssets(component[COMPONENT_DATA][MODEL_PATH].as<std::string>().c_str(), Resource::Type::MODEL);
            }

            component_mesh->LoadModel(component[COMPONENT_DATA][MODEL_PATH].as<std::string>().c_str());

            for (int i = 0; i < static_cast<int>(component[COMPONENT_DATA][MESH_NODE].size()); ++i)
            {
                // TODO: Components cant be (dis)enable yet
                //component[COMPONENT_DATA][MESH_NODE][i][MESH_ENABLED].as<bool>() ? component_mesh->EnableMesh(i) : component_mesh->DisableMesh(i);

                if (component[COMPONENT_DATA][MESH_NODE][i][MESH_TEXTURE].IsDefined())
                {
                    if (!CheckIfImported(component[COMPONENT_DATA][MESH_NODE][i][MESH_TEXTURE].as<std::string>().c_str()))
                    {
                        ImportFromAssets(component[COMPONENT_DATA][MESH_NODE][i][MESH_TEXTURE].as<std::string>().c_str(), Resource::Type::TEXTURE);
                    }

                    //component_mesh->LoadTexture(
                    //    component[COMPONENT_DATA][MESH_NODE][i][MESH_TEXTURE].as<std::string>().c_str(), i,
                    //    component[COMPONENT_DATA][MESH_NODE][i][MESH_TEXTURE_TYPE].as<int>());
                }
            }

            break;
        }
    case Component::Type::CAMERA:
        {
            ComponentCamera* component_camera = static_cast<ComponentCamera*>(game_object->CreateComponent(Component::Type::CAMERA));
            component_camera->SetID(component[COMPONENT_ID].as<size_t>());
            component[COMPONENT_ENABLED].as<bool>() ? component_camera->Enable() : component_camera->Disable();
            break;
        }
    case Component::Type::POINTLIGHT:
        {
            ComponentPointLight* component_point_light = static_cast<ComponentPointLight*>(game_object->CreateComponent(Component::Type::POINTLIGHT));

            component_point_light->SetID(component[COMPONENT_ID].as<size_t>());
            component[COMPONENT_ENABLED].as<bool>() ? component_point_light->Enable() : component_point_light->Disable();
            break;
        }
    }
}

void Hachiko::SceneSerializer::ImportFromAssets(const char* path, const Resource::Type type)
{
    App->importer->ImportAsset(path, type);
}

bool Hachiko::SceneSerializer::CheckIfImported(const char* path) const
{
    std::filesystem::path model_name(path);
    model_name = model_name.filename().replace_extension();
    std::string fullPath = preferences->GetLibraryPath(Resource::Type::MODEL);
    fullPath.append(model_name.string());

    return std::filesystem::exists(fullPath);
}
