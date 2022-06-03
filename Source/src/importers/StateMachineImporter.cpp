#include "core/hepch.h"

#include "StateMachineImporter.h"
#include "resources/ResourceStateMachine.h"

void Hachiko::StateMachineImporter::Import(const char* path, YAML::Node& meta)
{
    // Only 1 state machine per asset will exist
    static const int resource_index = 0;
    UID uid = ManageResourceUID(Resource::Type::STATE_MACHINE, resource_index, meta);

    FileSystem::Copy(path, GetResourcePath(Resource::Type::MATERIAL, uid).c_str());
}

void Hachiko::StateMachineImporter::Save(UID id, const Resource* resource)
{
    // State machine changes are also reflected in the asset file since they are an asset we can edit from engine
    const ResourceStateMachine* state_machine = static_cast<const ResourceStateMachine*>(resource);

    const std::string material_library_path = GetResourcePath(Resource::Type::STATE_MACHINE, id);
    
    const std::string asset_path = StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::AssetType::STATE_MACHINE), state_machine->state_m_name, STATE_MACHINE_EXTENSION);
    // TODO: Serialize state machine and save to asset path + to library path
    
    FileSystem::Copy(material_library_path.c_str(), material_library_path.c_str());
}

Hachiko::Resource* Hachiko::StateMachineImporter::Load(UID id)
{
    const std::string state_machine_library_path = GetResourcePath(Resource::Type::STATE_MACHINE, id);

    if (!FileSystem::Exists(state_machine_library_path.c_str()))
    {
        return nullptr;
    }

    
    ResourceStateMachine* state_machine = new ResourceStateMachine(id);

    // TODO: Load &Deserialize file
    YAML::Node node = YAML::LoadFile(state_machine_library_path);

    return state_machine;
}
