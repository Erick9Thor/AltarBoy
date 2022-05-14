#include "core/hepch.h"
#include "ModelImporter.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "AnimationImporter.h"

#include "resources/ResourceModel.h"
#include "core/preferences/src/ResourcesPreferences.h"
#include "modules/ModuleResources.h"

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

    const std::filesystem::path model_path(path);
    meta[MODEL_NAME] = model_path.filename().replace_extension().string();

    unsigned flags = aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_LimitBoneWeights
                     | aiProcess_SplitLargeMeshes | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType | aiProcess_FindDegenerates | aiProcess_FindInvalidData 
                     | aiProcess_GlobalScale;
    
    scene = import.ReadFile(path, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        HE_LOG("ERROR::ASSIMP::%c", import.GetErrorString());
        return;
    }

    ImportModel(path, scene, meta);
}



void Hachiko::ModelImporter::ImportModel(const char* path, const aiScene* scene, YAML::Node& meta)
{
    Hachiko::MeshImporter mesh_importer;
    Hachiko::MaterialImporter material_importer;
    Hachiko::AnimationImporter animation_importer;

    unsigned imported_materials = 0;
    unsigned imported_meshes = 0;
    unsigned imported_animaitons = 0;

    // Reset resources array
    meta.remove(RESOURCES);
        
    for (unsigned mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index)
    {
        Hachiko::UID mesh_id;
        if (ExistsInMetaArray(meta[MESHES], mesh_index))
        {
            mesh_id = meta[MESHES][mesh_index].as<UID>();
        }
        else
        {
            mesh_id = UUID::GenerateUID();
            meta[MESHES] = mesh_id;
        }
        aiMesh* mesh = scene->mMeshes[mesh_index];
        mesh_importer.ImportFromAssimp(mesh_id, mesh);
        AddResource(mesh_id, Resource::Type::MESH, meta);
    }

    for (unsigned material_index = 0; material_index < scene->mNumMaterials; ++material_index)
    {
        // Model importer decides theuid of new materials to keep them under control
        Hachiko::UID material_id;
        if (ExistsInMetaArray(meta[MATERIALS], material_index))
        {
            material_id = meta[MATERIALS][material_index].as<UID>();
        }
        else
        {
            material_id = UUID::GenerateUID();
            meta[MATERIALS] = material_id;
        }
        aiMaterial* material = scene->mMaterials[material_index];
        material_importer.CreateMaterialAssetFromAssimp(path, material, material_id);
        AddResource(material_id, Resource::Type::MATERIAL, meta);
    }

    if (scene->HasAnimations())
    {
        for (unsigned animation_index; animation_index < scene->mNumAnimations; ++animation_index)
        {
            Hachiko::UID animaiton_id;
            if (ExistsInMetaArray(meta[ANIMATIONS], animation_index))
            {
                animaiton_id = meta[ANIMATIONS][animation_index].as<UID>();
            }
            else
            {
                animaiton_id = UUID::GenerateUID();
                meta[ANIMATIONS] = animaiton_id;
            }
            aiAnimation* animation = scene->mAnimations[animation_index];
            animation_importer.CreateAnimationFromAssimp(animation, animaiton_id);
            AddResource(animaiton_id, Resource::Type::ANIMATION, meta);
        }
    }

    ImportNode(path, scene->mRootNode, meta, !scene->HasAnimations());
}

void Hachiko::ModelImporter::ImportNode(const char* path, const aiNode* assimp_node, YAML::Node& node, bool load_auxiliar)
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

        if (load_auxiliar)
        {
            if (node_name.find(AUXILIAR_NODE) != std::string::npos && assimp_node->mNumChildren == 1)
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

    }

    node[NODE_NAME] = assimp_node->mName.C_Str();
    node[NODE_TRANSFORM] = transform;

    for (unsigned int j = 0; j < assimp_node->mNumMeshes; ++j)
    {
        node[NODE_MESH_INDEX][j] = assimp_node->mMeshes[j];
    }

    for (unsigned i = 0; i < assimp_node->mNumChildren; ++i)
    {
        auto child_node = assimp_node->mChildren[i];
        ImportNode(child_node, node[NODE_CHILD][i], load_auxiliar);
        ++child_node;
    }
}

Hachiko::Resource* Hachiko::ModelImporter::Load(UID id)
{
    assert(id && "Unable to load module. Given an empty id");

    const std::string model_library_path = GetResourcePath(Resource::Type::MODEL, id);

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

    if (model_node[ANIMATIONS].IsDefined())
    {
        model_output->have_animation = model_node[ANIMATIONS].as<unsigned int>();

        model_output->materials.reserve(model_output->have_animation);

        for (unsigned i = 0; i < model_output->have_animation; ++i)
        {
            AnimationInfo animation_info;
            animation_info.animation_id = model_node[ANIMATION_IDS][i].as<UID>();
            model_output->animations.push_back(animation_info);
        }
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

void Hachiko::ModelImporter::Save(UID id, const Resource* resource) {}

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