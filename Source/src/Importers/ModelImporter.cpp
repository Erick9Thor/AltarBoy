#include "Core/hepch.h"
#include "ModelImporter.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"

#include "Modules/ModuleFileSystem.h"
#include "Core/preferences/src/ResourcesPreferences.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

Hachiko::ModelImporter::ModelImporter() : Importer(Importer::Type::MODEL)
{
}

void Hachiko::ModelImporter::Import(const char* path)
{
    HE_LOG("Entering ModelImporter: %s", path);

    if (preferences == nullptr)
    {
        preferences = static_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));
    }

    Assimp::Importer import;
    //import.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    const aiScene* scene = nullptr;
    const std::filesystem::path model_path(path);
    const std::string model_name = model_path.filename().replace_extension().string();
    const std::string model_library_path = preferences->GetLibraryPath(Resource::Type::MODEL) + model_name + MODEL_EXTENSION;
    scene = import.ReadFile(model_path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HE_LOG("ERROR::ASSIMP::%c", import.GetErrorString());
        return;
    }

    YAML::Node model_node;
    model_node[MODEL_ID] = UUID::GenerateUID();
    model_node[MODEL_FILE_PATH] = model_path.string();

    ImportModel(scene, model_node);
    // Import model textures

    App->file_sys->Save(model_library_path.c_str(), model_node);
}

Hachiko::Resource* Hachiko::ModelImporter::Load(const UID id)
{
    return nullptr;
}

Hachiko::Resource* Hachiko::ModelImporter::Load(const char* model_path)
{
    if (!App->file_sys->Exists(model_path))
    {
        return nullptr;
    }
    YAML::Node model_node = YAML::LoadFile(model_path);
    //Hachiko::Resource* model_output = new Resource(model_node[MODEL_ID].as<long long>(), Resource::Type::MODEL);

    return nullptr;
}

void Hachiko::ModelImporter::Save(const Resource* resource) {}

void Hachiko::ModelImporter::ImportModel(const aiScene* scene, YAML::Node& node)
{
    Hachiko::UID uid;
    Hachiko::MeshImporter mesh_importer;

    ImportNode(scene->mRootNode, node[NODE_ROOT]);

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        Hachiko::UID mesh_id = node[NODE_ROOT][NODE_CHILD][i][NODE_MESH_ID].as<UID>();
        node[MODEL_MESH_NODE][i][MODEL_MESH_ID] = mesh_id;
        mesh_importer.Import(mesh, mesh_id);
    }

    aiString file;
    Hachiko::MaterialImporter material_importer;
    for (unsigned i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial* material = scene->mMaterials[i];
        uid = Hachiko::UUID::GenerateUID();
        node[MODEL_MATERIAL_NODE][i][MODEL_MATERIAL_ID] = uid;
        material_importer.Import(material, uid);
    }
}

void Hachiko::ModelImporter::ImportNode(const aiNode* assimp_node, YAML::Node& node)
{
    node[NODE_NAME] = assimp_node->mName.C_Str();
    node[NODE_TRANSFORM] = assimp_node->mTransformation;
    node[NODE_MESH_ID] = Hachiko::UUID::GenerateUID();

    auto child_node = assimp_node->mChildren;
    for (unsigned i = 0; i < assimp_node->mNumChildren; ++i)
    {
        ImportNode(*child_node, node[NODE_CHILD][i]);
        ++child_node;
    }
}