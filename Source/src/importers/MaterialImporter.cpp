#include "core/hepch.h"

#include "MaterialImporter.h"

#include "modules/ModuleResources.h"
#include "modules/ModuleTexture.h"

#include "resources/ResourceMaterial.h"
#include "importers/TextureImporter.h"
#include "core/preferences/src/ResourcesPreferences.h"

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

void Hachiko::MaterialImporter::Import(const char* path, YAML::Node& meta)
{
    HE_LOG("Entering MaterialImporter: %s", path);
    YAML::Node material_node = YAML::LoadFile(path);
    FileSystem::Save(StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MATERIAL),
        meta[GENERAL_NODE][GENERAL_ID].as<std::string>()).c_str(), material_node);
}

void Hachiko::MaterialImporter::Save(const Resource* res) 
{
    const ResourceMaterial* material = static_cast<const ResourceMaterial*>(res);
    const std::string material_asset_path = 
        StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::Type::MATERIAL), material->GetName(), MATERIAL_EXTENSION);
    const std::string meta_path = 
        StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::Type::MATERIAL), material->GetName(),  MATERIAL_EXTENSION, META_EXTENSION);
    const std::string material_library_path = 
        StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MATERIAL), std::to_string(material->GetID()));
    
    YAML::Node meta_node;
    meta_node[GENERAL_NODE][GENERAL_ID] = material->GetID();
    meta_node[GENERAL_NODE][GENERAL_TYPE] = (int)material->GetType();

    YAML::Node material_node;
    material_node[MATERIAL_NAME] = material->GetName();

    material_node[MATERIAL_DIFFUSE_ID] = (material->HasDiffuse()) ? material->diffuse->GetID() : 0;
    material_node[MATERIAL_SPECULAR_ID] = (material->HasSpecular()) ? material->specular->GetID() : 0;
    material_node[MATERIAL_NORMALS_ID] = (material->HasNormal()) ? material->normal->GetID() : 0;
    material_node[MATERIAL_METALNESS_ID] = (material->HasMetalness()) ? material->metalness->GetID() : 0;

    material_node[MATERIAL_DIFFUSE_COLOR] = material->diffuse_color;
    material_node[MATERIAL_SPECULAR_COLOR] = material->specular_color;
    material_node[MATERIAL_SMOOTHNESS] = material->smoothness;
    material_node[MATERIAL_METALNESS_VALUE] = material->metalness_value;
    material_node[MATERIAL_IS_METALLIC] = material->is_metallic;
    material_node[MATERIAL_ALPHA_CHANNEL] = material->smoothness_alpha;
    material_node[MATERIAL_IS_TRANSPARENT] = material->is_transparent;

    
    FileSystem::Save(material_asset_path.c_str(), material_node);
    FileSystem::Save(material_library_path.c_str(), material_node);
    meta_node[GENERAL_NODE][GENERAL_HASH] = FileSystem::HashFromPath(material_asset_path.c_str());
    FileSystem::Save(meta_path.c_str(), meta_node);
}

Hachiko::Resource* Hachiko::MaterialImporter::Load(UID id)
{
    const std::string material_library_path = 
        GetResourcesPreferences()->GetLibraryPath(Resource::Type::MATERIAL) + std::to_string(id);

    YAML::Node node = YAML::LoadFile(material_library_path);
    Hachiko::ResourceMaterial* material = new ResourceMaterial(id);

    material->SetName(node[MATERIAL_NAME].as<std::string>());
    material->diffuse_color = node[MATERIAL_DIFFUSE_COLOR].as<float4>();
    material->specular_color = node[MATERIAL_SPECULAR_COLOR].as<float4>();
    material->smoothness = node[MATERIAL_SMOOTHNESS].as<float>();
    material->metalness_value = node[MATERIAL_METALNESS_VALUE].as<float>();
    material->is_metallic = node[MATERIAL_IS_METALLIC].as<unsigned>();
    material->smoothness_alpha = node[MATERIAL_ALPHA_CHANNEL].as<unsigned>();
    material->is_transparent = node[MATERIAL_IS_TRANSPARENT].as<unsigned>();

    UID texture_id = node[MATERIAL_DIFFUSE_ID].as<UID>();
    if (texture_id)
    {
        material->diffuse = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, texture_id));
    }

    texture_id = node[MATERIAL_SPECULAR_ID].as<UID>();
    if (texture_id)
    {
        material->specular = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, texture_id));
    }

    texture_id = node[MATERIAL_NORMALS_ID].as<UID>();
    if (texture_id)
    {
        material->normal = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, texture_id));
    }

    texture_id = node[MATERIAL_METALNESS_ID].as<UID>();
    if (texture_id)
    {
        material->metalness = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, texture_id));
    }

    return material;
}

void Hachiko::MaterialImporter::CreateMaterial(const std::string& name) 
{
    ResourceMaterial* material = new ResourceMaterial(Hachiko::UUID::GenerateUID());
    material->SetName(name);

    Save(material);

    delete material;
}

void Hachiko::MaterialImporter::Import(aiMaterial* ai_material, const UID id) 
{  
    const auto material = new ResourceMaterial(id);
    std::string name = ai_material->GetName().C_Str();
    if (name.find(':') != std::string::npos)
    {
        name.erase(remove(name.begin(), name.end(), ':'), name.end());
    }
    material->SetName(name);
    
    aiColor4D color;
    if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
    {
        ColorCopy(color, material->diffuse_color);
    }
    if (ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
    {
        ColorCopy(color, material->specular_color);
    }

    material->diffuse = ImportTexture(ai_material, aiTextureType_DIFFUSE);
    material->specular = ImportTexture(ai_material, aiTextureType_SPECULAR);
    material->normal = ImportTexture(ai_material, aiTextureType_NORMALS);
    material->metalness = ImportTexture(ai_material, aiTextureType_METALNESS);
    
    Save(material);

    delete material->diffuse;
    delete material->specular;
    delete material->normal;
    delete material->metalness;
    delete material;
}

Hachiko::ResourceTexture* Hachiko::MaterialImporter::ImportTexture(const aiMaterial* ai_material, aiTextureType type)
{
    static const int index = 0;
    aiString file;
    aiReturn ai_ret = ai_material->GetTexture(type, index, &file); 
    if ( ai_ret == AI_FAILURE || ai_ret == AI_OUTOFMEMORY)
    {
        return nullptr;
    }

    ResourceTexture* output_texture = nullptr;
    Hachiko::TextureImporter texture_importer;
    const std::string model_path = App->resources->GetLastResourceLoadedPath().u8string();
    const char* asset_path = App->preferences->GetResourcesPreference()->GetAssetsPath(Resource::Type::TEXTURE);
    std::vector<std::string> search_paths;
    const std::string model_texture_path(file.data);
    const std::string filename = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
    
    std::string meta_path = asset_path + filename;
    meta_path.append(META_EXTENSION);
    YAML::Node text_node;
    bool imported = false;

    search_paths.emplace_back(asset_path + filename);
    search_paths.emplace_back(model_path + "\\" + filename);
    search_paths.emplace_back(file.data);

    if (!FileSystem::Exists(meta_path.c_str()))
    {
        for (std::string& path : search_paths)
        {
            if (!std::filesystem::exists(path))
            {
                continue;
            }

            App->resources->HandleAssetFromAnyPath(path);
            text_node = YAML::LoadFile(meta_path);
            imported = true;
            break;
        }
    }
    else
    {
        text_node = YAML::LoadFile(meta_path);
        texture_importer.Import(search_paths[0].c_str(), text_node);
        imported = true;
    }

    if (imported)
    {
        UID id = text_node[GENERAL_NODE][GENERAL_ID].as<UID>();
        output_texture = static_cast<ResourceTexture*>(texture_importer.Load(id));
    }

    return output_texture;
}
