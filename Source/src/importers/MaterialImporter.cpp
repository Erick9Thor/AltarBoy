#include "core/hepch.h"
#include "MaterialImporter.h"

#include "Application.h"
#include "modules/ModuleResources.h"
#include "modules/ModuleFileSystem.h"
#include "resources/ResourceMaterial.h"
#include "core/preferences/src/ResourcesPreferences.h"
#include "core/serialization/TypeConverter.h"

#include "modules/ModuleTexture.h"
#include "importers/TextureImporter.h"

void ColorCopy(const aiColor4D& assimp_color, float4& color)
{
    color.x = assimp_color.r;
    color.y = assimp_color.g;
    color.z = assimp_color.b;
    color.w = assimp_color.a;
}

Hachiko::MaterialImporter::MaterialImporter() : Importer(Importer::Type::MATERIAL)
{
}

void Hachiko::MaterialImporter::Import(const char* path)
{
}

void Hachiko::MaterialImporter::Save(const Resource* res) 
{
    const ResourceMaterial* material = static_cast<const ResourceMaterial*>(res);
    const std::string material_library_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::MATERIAL) + std::to_string(material->GetID());

    YAML::Node material_node;
    material_node[MATERIAL_ID] = material->GetID();
    material_node[GENERIC_TYPE] = (int)material->GetType();
    material_node[MATERIAL_NAME] = material->name;
    material_node[MATERIAL_DIFFUSE] = (material->diffuse != nullptr) ? material->diffuse->GetID() : 0;
    material_node[MATERIAL_SPECULAR] = (material->specular != nullptr) ? material->specular->GetID() : 0;
    material_node[MATERIAL_NORMALS] = (material->normals != nullptr) ? material->normals->GetID() : 0;
    material_node[MATERIAL_DIFFUSE_COLOR] = YAML::convert<float4>::encode(material->diffuse_color);
    material_node[MATERIAL_SPECULAR_COLOR] = YAML::convert<float4>::encode(material->specular_color);
    material_node[MATERIAL_SHININESS] = material->shininess;

    App->file_sys->Save(material_library_path.c_str(), material_node);
}

Hachiko::Resource* Hachiko::MaterialImporter::Load(const UID uid)
{
    const std::string file_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::MATERIAL) + std::to_string(uid);
    if (!std::filesystem::exists(file_path.c_str()))
    {
        return nullptr;
    }

    char* file_buffer = App->file_sys->Load(file_path.c_str());
    YAML::Node node = YAML::Load(file_buffer);
    delete[] file_buffer;

    const auto material = new ResourceMaterial(uid);

    //node[MATERIAL_ID]
    //node[GENERIC_TYPE]
    material->name = node[MATERIAL_NAME].as<std::string>();
    UID diffuse_uid = node[MATERIAL_DIFFUSE].as<UID>();
    UID specular_uid = node[MATERIAL_SPECULAR].as<UID>();
    UID normals_uid = node[MATERIAL_NORMALS].as<UID>();
    YAML::convert<float4>::decode(node[MATERIAL_DIFFUSE_COLOR], material->diffuse_color);
    YAML::convert<float4>::decode(node[MATERIAL_SPECULAR_COLOR], material->specular_color);
    material->shininess = node[MATERIAL_SHININESS].as<float>();

    // Load texutes
    if (diffuse_uid)
    {
        material->diffuse = App->resources->GetTexture(diffuse_uid);
    }
    if (specular_uid)
    {
        material->specular = App->resources->GetTexture(specular_uid);
    }
    if (normals_uid)
    {
        material->normals = App->resources->GetTexture(normals_uid);
    }

    // Save it in module resources
    App->resources->materials[uid] = material;

    return material;
}

void Hachiko::MaterialImporter::Import(const aiMaterial* ai_material, const UID& id) 
{
    if (!id)
    {
        const_cast<UID&>(id) = UUID::GenerateUID();
    }

    const auto material = new ResourceMaterial(id);

    material->name = "NewMaterial";
    
    aiColor4D color;
    if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
    {
        ColorCopy(color, material->diffuse_color);
    }
    if (ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
    {
        ColorCopy(color, material->specular_color);
    }

    ai_material->Get(AI_MATKEY_SHININESS, material->shininess);

    static const int index = 0;
    const std::string model_path = App->resources->GetLastResourceLoadedPath().u8string();

    aiString file;
    std::vector<std::string> search_paths;

    Hachiko::TextureImporter texture_importer;

    // Load diffuse
    if (ai_material->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
    {
        const std::string model_texture_path(file.data);
        const std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
        search_paths.emplace_back(file.data);
        search_paths.push_back(model_path + texture_file);
        search_paths.push_back(ASSETS_FOLDER_TEXTURES + texture_file);

        for (std::string path : search_paths)
        {
            material->diffuse = static_cast<ResourceTexture*>(texture_importer.ImportResource(path.c_str()));
            if (material->diffuse != nullptr)
            {
                break;
            }
        }
        search_paths.clear();
    }

    // Load specular
    if (ai_material->GetTexture(aiTextureType_SPECULAR, index, &file) == AI_SUCCESS)
    {
        const std::string model_texture_path(file.data);
        const std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
        search_paths.emplace_back(file.data);
        search_paths.push_back(model_path + texture_file);
        search_paths.push_back(ASSETS_FOLDER_TEXTURES + texture_file);
        
        for (std::string path : search_paths)
        {
            material->specular = static_cast<ResourceTexture*>(texture_importer.ImportResource(path.c_str()));

            if (material->specular != nullptr)
            {
                break;
            }
        }
    }

    // Load normals
    if (ai_material->GetTexture(aiTextureType_NORMALS, index, &file) == AI_SUCCESS)
    {
        const std::string model_texture_path(file.data);
        const std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
        search_paths.emplace_back(file.data);
        search_paths.push_back(model_path + texture_file);
        search_paths.push_back(ASSETS_FOLDER_TEXTURES + texture_file);

        for (std::string path : search_paths)
        {
            material->normals = static_cast<ResourceTexture*>(texture_importer.ImportResource(path.c_str()));
            if (material->normals != nullptr)
            {
                break;
            }
        }
    }

    Save(material);

    delete material->diffuse;
    delete material->specular;
    delete material->normals;
    delete material;
}