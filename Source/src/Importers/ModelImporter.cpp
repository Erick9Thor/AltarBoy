#include "Core/hepch.h"
#include "ModelImporter.h"
#include "MeshImporter.h"

#include "Modules/ModuleFileSystem.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

Hachiko::ModelImporter::ModelImporter() : Importer(Importer::Type::MODEL) {}

void Hachiko::ModelImporter::Import(const char* path)
{
    HE_LOG("Entering ModelImporter: %s", path);
    Assimp::Importer import;
    const aiScene* scene = nullptr;
    const std::string model_path(path);

    scene = import.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HE_LOG("ERROR::ASSIMP::%c", import.GetErrorString());
        return;
    }

    YAML::Node model_node;
    model_node[MODEL_ID] = UUID::GenerateUID();
    model_node[MODEL_FILE_PATH] = model_path;

    ImportModel(scene, model_node);
    // Import model textures

    App->file_sys->Save(model_node, model_path);
}

void Hachiko::ModelImporter::Load(UID id) {}

void Hachiko::ModelImporter::Save()
{
}

void Hachiko::ModelImporter::ImportModel(const aiScene* scene, YAML::Node& node)
{
    Hachiko::UID mesh_id;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        mesh_id = Hachiko::UUID::GenerateUID();
        node[MODEL_MESH_NODE][i][MODEL_MESH_ID] = mesh_id;
        Hachiko::MeshImporter::Import(mesh, mesh_id);
    }
}