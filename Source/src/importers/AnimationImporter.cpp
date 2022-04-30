#include "core/hepch.h"
#include "AnimationImporter.h"

#include "core/preferences/src/ResourcesPreferences.h"

#include "resources/ResourceAnimation.h"

Hachiko::AnimationImporter::AnimationImporter() : Importer(Importer::Type::ANIMATION) 
{}

void Hachiko::AnimationImporter::Import(const char* path, YAML::Node& meta) 
{
    HE_LOG("Entering Animation Importer: %s", path);
    Assimp::Importer import;

    const aiScene* scene = nullptr;
    const std::filesystem::path material_path(path);

    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    assert(scene->mNumAnimations == 1);

    Import(scene->mAnimations[0], meta[GENERAL_NODE][GENERAL_ID].as<UID>());
}

void Hachiko::AnimationImporter::ImportWithMeta(const char* path, YAML::Node& meta) 
{
    Import(path, meta);
}

void Hachiko::AnimationImporter::Save(const Resource* resource) 
{
    const std::string animation_library_path = GetResourcesPreferences()->GetAssetsPath(Resource::Type::ANIMATION) + resource->GetID();

    // TO IMPLEMENT
}

Hachiko::Resource* Hachiko::AnimationImporter::Load(UID id)
{
    assert(id && "Unable to load module. Given an empty id");

    const std::string animation_library_path = StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::ANIMATION), std::to_string(id));

    char* file_buffer = FileSystem::Load(animation_library_path.c_str());

    const auto animation = new ResourceAnimation(id);

    /* TODO: ANIMATION add all info to charge the node into resource */

    delete[] file_buffer;

    return animation;
}

void Hachiko::AnimationImporter::Import(const aiAnimation* animation, UID id)
{
    const auto r_animation = new ResourceAnimation(id);

    r_animation->SetDuration(unsigned(1000 * animation->mDuration / animation->mTicksPerSecond));

    r_animation->channels.reserve(animation->mNumChannels);

    for (unsigned i = 0; i < animation->mNumChannels; ++i)
    {
        const aiNodeAnim* node = animation->mChannels[i];
        ResourceAnimation::Channel& channel = r_animation->channels[std::string(node->mNodeName.C_Str())];

        unsigned int pos_first = 0;
        unsigned int pos_last = 1;

        unsigned int rot_first = 0;
        unsigned int rot_last = 1;

        if (node->mNumPositionKeys > 1)
        {
            pos_first = node->mNumPositionKeys;
            pos_last = std::max(pos_first, node->mNumPositionKeys);
        }

        if (node->mNumRotationKeys > 1)
        {
            rot_first = node->mNumRotationKeys;
            rot_last = std::max(rot_first, node->mNumRotationKeys);
        }

        channel.num_positions = pos_last - pos_first;
        channel.num_rotations = rot_last - rot_first;

        channel.positions = std::make_unique<float3[]>(channel.num_positions);
        channel.rotations = std::make_unique<Quat[]>(channel.num_rotations);

        for (unsigned j = 0; j < (pos_last - pos_first); ++j)
        {
            channel.positions[j] = (*reinterpret_cast<math::float3*>(&node->mPositionKeys[j + pos_first].mValue)) * 1.0f;
        }

        for (unsigned j = 0; j < (rot_last - rot_first); ++j)
        {
            const aiQuaternion& quat = node->mRotationKeys[j + rot_first].mValue;
            channel.rotations[j] = Quat(quat.x, quat.y, quat.z, quat.w);
        }
    }

    Save(r_animation);
}