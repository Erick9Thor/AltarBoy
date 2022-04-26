#include "core/hepch.h"
#include "ModelImporter.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"

#include "resources/ResourceModel.h"
#include "core/preferences/src/ResourcesPreferences.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

Hachiko::ModelImporter::ModelImporter() : Importer(Importer::Type::MODEL) {}

void Hachiko::ModelImporter::Import(const char* path, YAML::Node& meta)
{
    HE_LOG("Entering ModelImporter: %s", path);
    Assimp::Importer import;
    import.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    const aiScene* scene = nullptr;

    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GlobalScale | aiProcess_CalcTangentSpace);
    
    std::filesystem::path mp(path);
    meta[MODEL_NAME] = mp.filename().replace_extension().string();

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HE_LOG("ERROR::ASSIMP::%c", import.GetErrorString());
        return;
    }

    ImportModel(scene, meta);
    FileSystem::Save(StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MODEL), meta[GENERAL_NODE][GENERAL_ID].as<std::string>()).c_str(), meta);
}

void Hachiko::ModelImporter::ImportWithMeta(const char* path, YAML::Node& meta) 
{
    const std::filesystem::path model(path);

    // 1 - Open assimp model
    Assimp::Importer import;
    const aiScene* scene = nullptr;
    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GlobalScale | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HE_LOG("ERROR::ASSIMP::%c", import.GetErrorString());
        return;
    }

    // 2 - Import Meshes
    MeshImporter mesh_importer;
    for (int i = 0; i < meta[MODEL_MESH_NODE].size(); ++i)
    {
        mesh_importer.Import(scene->mMeshes[i], meta[MODEL_MESH_NODE][i][MODEL_MESH_ID].as<UID>());
    }

    FileSystem::Save(StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MODEL), meta[GENERAL_NODE][GENERAL_ID].as<std::string>()).c_str(), meta);
}

void Hachiko::ModelImporter::ImportModel(const aiScene* scene, YAML::Node& node)
{
    Hachiko::MeshImporter mesh_importer;
    Hachiko::MaterialImporter material_importer;

    for (unsigned i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial* material = scene->mMaterials[i];
        Hachiko::UID material_id = UUID::GenerateUID();
        node[MODEL_MATERIAL_NODE][i][MODEL_MATERIAL_ID] = material_id;
        node[MODEL_MATERIAL_NODE][i][MATERIAL_NAME] = scene->mMaterials[i]->GetName().C_Str();
        material_importer.Import(material, material_id);
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[i];
        Hachiko::UID mesh_id = UUID::GenerateUID();
        node[MODEL_MESH_NODE][i][MODEL_MESH_ID] = mesh_id;
        node[MODEL_MESH_NODE][i][NODE_MATERIAL_INDEX] = mesh->mMaterialIndex;
        mesh_importer.Import(mesh, mesh_id);
    }

    ImportNode(scene->mRootNode, node);
}

void Hachiko::ModelImporter::ImportNode(const aiNode* assimp_node, YAML::Node& node)
{
    std::string node_name = assimp_node->mName.C_Str();

    aiVector3D aiTranslation, aiScale;
    aiQuaternion aiRotation;
    assimp_node->mTransformation.Decompose(aiScale, aiRotation, aiTranslation);
    float3 pos(aiTranslation.x, aiTranslation.y, aiTranslation.z);
    Quat rot(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);
    float3 scale(aiScale.x, aiScale.y, aiScale.z);

    float4x4 transform = float4x4::FromTRS(pos, rot, scale);

    bool dummy_node = true;
    while (dummy_node)
    {
        dummy_node = false;
        if (node_name.find("_$AssimpFbx$_") != std::string::npos && assimp_node->mNumChildren == 1)
        {
            assimp_node = assimp_node->mChildren[0];
            assimp_node->mTransformation.Decompose(aiScale, aiRotation, aiTranslation);

            pos = float3(aiTranslation.x, aiTranslation.y, aiTranslation.z);
            rot = Quat(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);
            scale = float3(aiScale.x, aiScale.y, aiScale.z);

            transform = transform * float4x4::FromTRS(pos, rot, scale);;

            node_name = assimp_node->mName.C_Str();
            dummy_node = true;
        }
    }

    node[NODE_NAME] = assimp_node->mName.C_Str();
    node[NODE_TRANSFORM] = transform;

    for (unsigned int j = 0; j < assimp_node->mNumMeshes; ++j)
    {
        node[NODE_MESH_INDEX][j] = assimp_node->mMeshes[j];
    }

    auto child_node = assimp_node->mChildren;
    for (unsigned i = 0; i < assimp_node->mNumChildren; ++i)
    {
        ImportNode(*child_node, node[NODE_CHILD][i]);
        ++child_node;
    }
}

Hachiko::Resource* Hachiko::ModelImporter::Load(UID id)
{
    assert(id && "Unable to load module. Given an empty id");

    const std::string model_library_path = StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MODEL), std::to_string(id));
    
    YAML::Node model_node = YAML::LoadFile(model_library_path);
    Hachiko::ResourceModel* model_output = new ResourceModel(model_node[GENERAL_NODE][GENERAL_ID].as<UID>());
    
    model_output->model_name = model_node[MODEL_NAME].as<std::string>();
    model_output->meshes.reserve(model_node[MODEL_MESH_NODE].size());
    for (unsigned i = 0; i < model_node[MODEL_MESH_NODE].size(); ++i)
    {
        MeshInfo mesh_info;
        mesh_info.mesh_id = model_node[MODEL_MESH_NODE][i][MODEL_MESH_ID].as<UID>();
        mesh_info.material_index = model_node[MODEL_MESH_NODE][i][NODE_MATERIAL_INDEX].as<int>();
        model_output->meshes.push_back(mesh_info);
    }

    model_output->materials.reserve(model_node[MODEL_MATERIAL_NODE].size());
    for (unsigned i = 0; i < model_node[MODEL_MATERIAL_NODE].size(); ++i)
    {
        MaterialInfo material_info;
        material_info.material_id = model_node[MODEL_MATERIAL_NODE][i][MODEL_MATERIAL_ID].as<UID>();
        material_info.material_name = model_node[MODEL_MATERIAL_NODE][i][MATERIAL_NAME].as<std::string>();
        model_output->materials.push_back(material_info);
    }

    LoadChildren(model_node[NODE_CHILD], model_node[MODEL_MESH_NODE], model_node[MODEL_MATERIAL_NODE], model_output->child_nodes);
    return model_output;
}

void Hachiko::ModelImporter::LoadChildren(YAML::Node& node, YAML::Node& meshes, YAML::Node& materials, std::vector<ResourceNode*>& children)
{
    children.reserve(node.size());
    for (int i = 0; i < node.size(); ++i)
    {
        ResourceNode* resource_node = new ResourceNode();
        resource_node->node_name = node[i][NODE_NAME].as<std::string>();
        resource_node->node_transform = node[i][NODE_TRANSFORM].as<float4x4>();
        //resource_node->node_transform = float4x4::FromTRS(node[i][TRANSFORM_POSITION].as<float3>(), node[i][TRANSFORM_ROTATION].as<Quat>(), node[i][TRANSFORM_SCALE].as<float3>());

        for (int j = 0; j < node[i][NODE_MESH_INDEX].size(); ++j)
        {
            int mesh_idx = node[i][NODE_MESH_INDEX][0].as<int>();
            int material_idx = meshes[mesh_idx][NODE_MATERIAL_INDEX].as<int>();

            resource_node->meshes_index.push_back(node[i][NODE_MESH_INDEX][j].as<int>());
        }

        if (node[i][NODE_CHILD].IsDefined())
        {
            LoadChildren(node[i][NODE_CHILD], meshes, materials, resource_node->children);
        }

        children.emplace_back(resource_node);
    }
}

void Hachiko::ModelImporter::Save(const Resource* resource) {}

//Hachiko::Resource* Hachiko::ModelImporter::CherryImport(int mesh_index, const UID uid, const char* model_path)
//{
//    HE_LOG("ModelImporter::CherryImport");
//    HE_LOG("Model path: %s", model_path);
//    HE_LOG("UID: %ull", uid);
//    HE_LOG("Mesh index: %d", mesh_index);
//
//    const std::filesystem::path model(model_path);
//  
//    // 1 - Open assimp model
//    Assimp::Importer import;
//    const aiScene* scene = nullptr;
//    scene = import.ReadFile(model.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//    {
//        HE_LOG("ERROR::ASSIMP::%c", import.GetErrorString());
//        return nullptr;
//    }
//
//    // 2 - Open model meta if exists. If not, create one
//    const std::string model_name = model.filename().replace_extension().string();
//    const std::string model_library_path = StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MODEL), model_name, META_EXTENSION);
//    YAML::Node model_node;
//
//    if (!std::filesystem::exists(model_library_path))
//    {
//        model_node[GENERAL_NODE][GENERAL_ID] = UUID::GenerateUID();
//
//        ImportNode(scene->mRootNode, model_node);
//    }
//    else
//    {
//        model_node = YAML::LoadFile(model_library_path);
//    }
//
//    model_node[MODEL_MESH_NODE][mesh_index][MODEL_MESH_ID] = uid;
//
//    // 3 - Import Single Mesh
//    MeshImporter mesh_importer;
//    mesh_importer.Import(scene->mMeshes[mesh_index], uid);
//
//    // 4- If ok, save new model data
//    FileSystem::Save(model_library_path.c_str(), model_node);
//
//    return mesh_importer.Load(uid);
//}

void Hachiko::ModelImporter::Delete(const YAML::Node& meta) 
{
    for (unsigned i = 0; i < meta[MODEL_MESH_NODE].size(); ++i)
    {
        UID mesh_uid = meta[MODEL_MESH_NODE][i][MODEL_MESH_ID].as<UID>();
        std::string mesh_library_path = StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MESH), std::to_string(mesh_uid));

        FileSystem::Delete(mesh_library_path.c_str());
    }

    Importer::Delete(meta);
}

bool Hachiko::ModelImporter::IsImported(const char* path)
{
    const std::filesystem::path model(path);
    return std::filesystem::exists(StringUtils::Concat(model.parent_path().string(), "\\", model.filename().replace_extension(META_EXTENSION).string()));
}