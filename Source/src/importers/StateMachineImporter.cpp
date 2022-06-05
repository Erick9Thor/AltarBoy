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
    /* const ResourceStateMachine* state_machine = static_cast<const ResourceStateMachine*>(resource);
    const std::string sm_library_path = GetResourcePath(Resource::Type::STATE_MACHINE, id);

    unsigned file_size = 0;

    unsigned header[3] = {
        state_machine->GetNumClips(), 
        state_machine->GetNumNodes(), 
        state_machine->GetNumTransitions()
    };

    file_size += sizeof(header) + state_machine->GetName().length() + sizeof(unsigned int);

    // Save Clips

    std::vector<unsigned> clips_header(3 * (double)state_machine->GetNumClips());
    unsigned i = 0;
    for (auto it = state_machine->clips.begin(); it != state_machine->clips.end(); ++it)
    {
        
    }

    file_size += sizeof(unsigned) * clips_header.size();

    // Save Nodes
    std::vector<unsigned> nodes_header(4 * (double)state_machine->GetNumNodes());
    for (auto it = state_machine->clips.begin(); it != state_machine->clips.end(); ++it)
    {
    
    }

    file_size += sizeof(unsigned) * nodes_header.size();

    // Save Transitions
    std::vector<unsigned> transitions_header(4 * (double)state_machine->GetNumTransitions());
    for (auto it = state_machine->clips.begin(); it != state_machine->clips.end(); ++it)
    {
    
    }

    file_size += sizeof(unsigned) * transitions_header.size();

    char* file_buffer = new char[file_size];
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    size_bytes = sizeof(header);
    memcpy(cursor, header, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(unsigned) * clips_header.size();
    memcpy(cursor, &clips_header[0], size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(unsigned) * nodes_header.size();
    memcpy(cursor, &nodes_header[0], size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(unsigned) * transitions_header.size();
    memcpy(cursor, &transitions_header[0], size_bytes);
    cursor += size_bytes;

    const std::string asset_path = StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::AssetType::STATE_MACHINE), state_machine->state_m_name, STATE_MACHINE_EXTENSION);
    FileSystem::Copy(asset_path.c_str(), sm_library_path.c_str());*/
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
