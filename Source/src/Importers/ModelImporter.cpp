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
    const std::string model_library_path = preferences->GetLibraryPath(Resource::Type::MODEL) + model_name;
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

void Hachiko::ModelImporter::Save(const Resource* resource) {}

void Hachiko::ModelImporter::ImportModel(const aiScene* scene, YAML::Node& node)
{
    Hachiko::UID uid;
    Hachiko::MeshImporter mesh_importer;
    Hachiko::MaterialImporter material_importer;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        uid = Hachiko::UUID::GenerateUID();
        node[MODEL_MESH_NODE][i][MODEL_MESH_ID] = uid;
        mesh_importer.Import(mesh, uid);
    }

    /// WORK IN PROGRESS
    aiString file;
    for (unsigned i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial* material = scene->mMaterials[i];
        uid = Hachiko::UUID::GenerateUID();
        node[MODEL_MATERIAL_NODE][i][MODEL_MATERIAL_ID] = uid;
        material_importer.Import(material, uid);
    }
}