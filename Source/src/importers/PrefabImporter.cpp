#include "core/hepch.h"

#include "PrefabImporter.h"
#include "resources/ResourcePrefab.h"
#include "modules/ModuleResources.h"
#include "modules/ModuleSceneManager.h"
#include "scripting/Script.h"
#include "core/Scene.h"

void Hachiko::PrefabImporter::Import(const char* path, YAML::Node& meta)
{
    // Only 1 prefab per prefab asset will exist
    static const int resource_index = 0;
    UID uid = ManageResourceUID(Resource::Type::PREFAB, resource_index, meta);

    YAML::Node prefab_node = YAML::LoadFile(path);
    FileSystem::Save(GetResourcePath(Resource::Type::PREFAB, uid).c_str(), prefab_node);
}

void Hachiko::PrefabImporter::Save(UID id, const Resource* prefab)
{
    const ResourcePrefab* resource = static_cast<const ResourcePrefab*>(prefab);
    
    const std::string resource_path = GetResourcePath(Resource::Type::PREFAB, id);    
    const std::string prefab_asset_path = StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::AssetType::PREFAB), resource->name, PREFAB_EXTENSION);
    FileSystem::Save(resource_path.c_str(), resource->prefab_data);
    FileSystem::Save(prefab_asset_path.c_str(), resource->prefab_data);
}

Hachiko::Resource* Hachiko::PrefabImporter::Load(UID id)
{
    const std::string prefab_resource_path = GetResourcePath(Resource::Type::PREFAB, id);
    YAML::Node node = YAML::LoadFile(prefab_resource_path);
    ResourcePrefab* prefab = new ResourcePrefab(id);
    prefab->prefab_data = node;
    prefab->name = node[PREFAB_NAME].as<std::string>();

    Scene* scene = App->scene_manager->GetActiveScene();
    GameObject* prefab_root = scene->CreateNewGameObject(nullptr, prefab->name.c_str());

    // Load generating new ids
    constexpr bool as_prefab = true;
    prefab_root->Load(prefab->prefab_data, as_prefab);

    // Wire script references by indexes
    std::vector<const GameObject*> object_collector = std::vector<const GameObject*>();
    std::vector<const Component*> component_collector = std::vector<const Component*>();
    // add a nullptr to 0 to reserve index 0 for missing references
    object_collector.push_back(nullptr);
    component_collector.push_back(nullptr);

    prefab_root->CollectObjectsAndComponents(object_collector, component_collector);
    // For load references we dont need to pass the yaml data since it is already parsed
    // And assigned to each script on their normal load
    prefab_root->LoadPrefabReferences(object_collector, component_collector);

    for (auto script : prefab_root->GetComponents(Component::Type::SCRIPT))
    {
        static_cast<Scripting::Script*>(script)->OnLoad();
    }
    for (auto script : prefab_root->GetComponentsInDescendants(Component::Type::SCRIPT))
    {
        static_cast<Scripting::Script*>(script)->OnLoad();
    }

    delete prefab;
    // We dont use the prefab resource outside of load
    return nullptr;
}

Hachiko::UID Hachiko::PrefabImporter::CreatePrefabAsset(const char* name, GameObject* root)
{
    // This id wont be used
    ResourcePrefab* prefab = new ResourcePrefab(0);
    prefab->name = name;
    prefab->prefab_data[PREFAB_NAME] = prefab->name;

    // Save without storing ids
    constexpr bool as_prefab = true;    
    root->Save(prefab->prefab_data, as_prefab);   


    // Wire script references by indexes
    std::vector<const GameObject*> object_collector = std::vector<const GameObject*>();
    std::vector<const Component*> component_collector = std::vector<const Component*>();
    // add a nullptr to 0 to reserve index 0 for missing references
    object_collector.push_back(nullptr);
    component_collector.push_back(nullptr);

    root->CollectObjectsAndComponents(object_collector, component_collector);
    root->SavePrefabReferences(prefab->prefab_data, object_collector, component_collector);
    
    const std::string prefab_asset_path = StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::AssetType::PREFAB), prefab->name, PREFAB_EXTENSION);
    FileSystem::Save(prefab_asset_path.c_str(), prefab->prefab_data);
    UID prefab_uid = App->resources->ImportAssetFromAnyPath(prefab_asset_path)[0];
    delete prefab;
    return prefab_uid;
}