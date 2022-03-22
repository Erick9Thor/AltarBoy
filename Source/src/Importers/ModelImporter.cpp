#include "Core/hepch.h"
#include "ModelImporter.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"

#include "resources/ResourceModel.h"
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
    Assimp::Importer import;
    //import.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    const aiScene* scene = nullptr;
    const std::filesystem::path model_path(path);
    const std::string model_name = model_path.filename().replace_extension().string();
    const std::string model_library_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::MODEL) + model_name + MODEL_EXTENSION;
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
    if (!std::filesystem::exists(model_path))
    {
        return nullptr;
    }
    YAML::Node model_node = YAML::LoadFile(model_path);
    Hachiko::ResourceModel* model_output = new ResourceModel(model_node[MODEL_ID].as<UID>());
    model_output->model_path = model_node[MODEL_FILE_PATH].as<std::string>();
    std::filesystem::path mp = model_node[MODEL_FILE_PATH].as<std::string>();
    model_output->model_name = mp.filename().replace_extension().string();
    LoadChilds(model_node[NODE_CHILD], model_output->child_nodes);
    return model_output;
}

void Hachiko::ModelImporter::Save(const Resource* resource) {}

void Hachiko::ModelImporter::ImportModel(const aiScene* scene, YAML::Node& node)
{
    Hachiko::MeshImporter mesh_importer;
    Hachiko::MaterialImporter material_importer;
    
    for (unsigned i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial* material = scene->mMaterials[i];
        Hachiko::UID material_id = UUID::GenerateUID();
        node[MODEL_MATERIAL_NODE][i][MODEL_MATERIAL_ID] = material_id;
        material_importer.Import(material, material_id);
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        Hachiko::UID mesh_id = UUID::GenerateUID();
        node[MODEL_MESH_NODE][i][MODEL_MESH_ID] = mesh_id;
        mesh_importer.Import(mesh, mesh_id);
    }

    ImportNode(scene->mRootNode, node);
}

void Hachiko::ModelImporter::ImportNode(const aiNode* assimp_node, YAML::Node& node)
{
    node[NODE_NAME] = assimp_node->mName.C_Str();
    node[NODE_TRANSFORM] = assimp_node->mTransformation;
    
    auto child_node = assimp_node->mChildren;
    for (unsigned i = 0; i < assimp_node->mNumChildren; ++i)
    {
        ImportNode(*child_node, node[NODE_CHILD][i]);
        node[NODE_CHILD][i][NODE_MESH_ID] = node[MODEL_MESH_NODE][i][MODEL_MESH_ID].as<UID>();
        // Applies the same material to all
        node[NODE_CHILD][i][NODE_MATERIAL_ID] = node[MODEL_MATERIAL_NODE][0][MODEL_MATERIAL_ID].as<UID>(); 
        ++child_node;
    }
}

void Hachiko::ModelImporter::LoadChilds(YAML::Node& node, std::vector<ResourceNode*>& childs)
{
    childs.reserve(node.size());
    for (int i = 0; i < node.size(); ++i)
    {
        ResourceNode* resource_node = new ResourceNode();
        resource_node->mesh_id = node[i][NODE_MESH_ID].as<UID>();
        resource_node->material_id = node[i][NODE_MATERIAL_ID].as<UID>();
        resource_node->node_name = node[i][NODE_NAME].as<std::string>();
        resource_node->node_transform = node[i][NODE_TRANSFORM].as<float4x4>();
        if (node[i][NODE_CHILD].IsDefined())
        {
            LoadChilds(node[i][NODE_CHILD], resource_node->childs);
        }

        childs.emplace_back(resource_node);
    }
}
