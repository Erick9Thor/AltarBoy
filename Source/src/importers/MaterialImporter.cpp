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
    material_node[MATERIAL_DIFFUSE_COLOR] = material->diffuse_color;
    material_node[MATERIAL_SPECULAR_COLOR] = material->specular_color;
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

    YAML::Node node = YAML::LoadFile(file_path.c_str());
    const auto material = new ResourceMaterial(uid);

    //node[MATERIAL_ID]
    //node[GENERIC_TYPE]
    material->name = node[MATERIAL_NAME].as<std::string>();
    UID diffuse_uid = node[MATERIAL_DIFFUSE].as<UID>();
    UID specular_uid = node[MATERIAL_SPECULAR].as<UID>();
    UID normals_uid = node[MATERIAL_NORMALS].as<UID>();
    material->diffuse_color = node[MATERIAL_DIFFUSE_COLOR].as<float4>();
    material->specular_color = node[MATERIAL_SPECULAR_COLOR].as<float4>();
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

    return material;
}

Hachiko::Resource* Hachiko::MaterialImporter::CherryImport(UID uid, const char* material_path)
{
    return nullptr;
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

    material->diffuse = ImportTexture(ai_material, aiTextureType_DIFFUSE);
    material->specular = ImportTexture(ai_material, aiTextureType_SPECULAR);
    material->normals = ImportTexture(ai_material, aiTextureType_NORMALS);

    Save(material);

    delete material->diffuse;
    delete material->specular;
    delete material->normals;
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
    
    search_paths.emplace_back(asset_path + filename);
    search_paths.emplace_back(model_path + "\\" + filename);
    search_paths.emplace_back(file.data);

    for (std::string& path : search_paths)
    {
        output_texture = static_cast<ResourceTexture*>(texture_importer.ImportTexture(path.c_str()));
        if (output_texture == nullptr)
        {
            continue;
        }
            
        if (path != (asset_path + filename))
        {
            std::filesystem::copy_file(path.c_str(), (asset_path + filename),
                std::filesystem::copy_options::skip_existing);
        }

        break;
    }

    return output_texture;
}
