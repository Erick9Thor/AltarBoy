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
    // Only 1 material will exist
    static const int resource_index = 0;
    UID uid = App->resources->ManageResourceUID(resource_index, meta);

    YAML::Node material_node = YAML::LoadFile(path);
    FileSystem::Save(StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MATERIAL),
        uid).c_str(), material_node);
}

void Hachiko::MaterialImporter::Save(const Resource* res) 
{
    // Material changes are also reflected in the asset file
    const ResourceMaterial* material = static_cast<const ResourceMaterial*>(res);
    const std::string material_asset_path = 
        StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::Type::MATERIAL), material->GetName(), MATERIAL_EXTENSION);
    const std::string material_library_path = 
        StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(Resource::Type::MATERIAL), std::to_string(material->GetID()));

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

void Hachiko::MaterialImporter::CreateEmptyMaterial(const std::string& name) 
{
    ResourceMaterial* material = new ResourceMaterial(Hachiko::UUID::GenerateUID());
    material->SetName(name);

    Save(material);

    delete material;
}

void Hachiko::MaterialImporter::CreateMaterialAssetFromAssimp(const std::string& model_path, aiMaterial* ai_material, UID uid)
{  
    const auto material = new ResourceMaterial(uid);
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

    Hachiko::TextureImporter texture_importer;
    material->diffuse = texture_importer.CreateTextureAssetFromAssimp(model_path, ai_material, aiTextureType_DIFFUSE);
    material->specular = texture_importer.CreateTextureAssetFromAssimp(model_path, ai_material, aiTextureType_SPECULAR);
    material->normal = texture_importer.CreateTextureAssetFromAssimp(model_path, ai_material, aiTextureType_NORMALS);
    material->metalness = texture_importer.CreateTextureAssetFromAssimp(model_path, ai_material, aiTextureType_METALNESS);
    
    Save(material);

    delete material->diffuse;
    delete material->specular;
    delete material->normal;
    delete material->metalness;
    delete material;
}
