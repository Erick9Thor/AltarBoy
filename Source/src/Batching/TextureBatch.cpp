#include "core/hepch.h"
#include "TextureBatch.h"

#include "core/GameObject.h"
#include "components/ComponentMesh.h"
#include "components/ComponentMaterial.h"
#include "resources/ResourceMaterial.h"

#include "Modules/ModuleProgram.h"
#include "Modules/ModuleTexture.h"

Hachiko::TextureBatch::TextureBatch() 
{
}

Hachiko::TextureBatch::~TextureBatch() 
{
    for (auto& resource : resources)
    {
        delete resource.second;
    }
    resources.clear();

    for (TextureArray* textureArray : textureArrays)
    {
        glDeleteTextures(textureArray->depth, &textureArray->id);
        delete textureArray;
    }
    textureArrays.clear();
}

void Hachiko::TextureBatch::AddMaterial(const ComponentMaterial* material)
{
    const ResourceMaterial* resource_material = material->GetMaterial();

    if (resource_material->HasDiffuse())
    {
        AddTexture(resource_material->diffuse);
    }
    if (resource_material->HasSpecular())
    {
        AddTexture(resource_material->specular);
    }
}

void Hachiko::TextureBatch::AddTexture(const ResourceTexture* texture) 
{
    auto it = resources.find(texture);

    if (it == resources.end())
    {
        // It will be formatted when generating batches
        resources[texture] = new TexAddress();

        // Search for the its textureArray
        bool textureArrayFound = false;
        for (TextureArray* textureArray : textureArrays)
        {
            if (EqualLayout(*textureArray, *texture))
            {
                textureArrayFound = true;
                textureArray->depth += 1;
                break;
            }
        }

        // If there is none, create it
        if (!textureArrayFound)
        {
            TextureArray* textureArray = new TextureArray();
            textureArray->depth = 1;
            textureArray->width = texture->width;
            textureArray->height = texture->height;
            textureArray->format = texture->format;

            textureArrays.push_back(textureArray);
        }

        // Set the flag as not all textures are loaded
        loaded = false;
    }
}

void Hachiko::TextureBatch::GenerateBatch() 
{
    // TODO: improve performance (for inside for)

    int maxLayers; // maximum number of array texture layers 
    int maxUnits; // maximum number of texture units
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);

    // Security check, amount of texture units
    if (textureArrays.size() > maxUnits - 8)
    {
        HE_LOG("[Error] TextureBatch: there are more texture units than the maximum.");
    }

    unsigned index = 0;
    for (unsigned i = 0; i < textureArrays.size(); ++i)
    {
        // Security check, amount of layers in a texture array
        if (textureArrays[i]->depth > maxLayers)
        {
            HE_LOG("[Error] TextureBatch: there are layers in a texture array than the maximum.");
        }

        glGenTextures(1, &textureArrays[i]->id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrays[i]->id);

        // Generate texture array
        unsigned sizedFormat = GL_RGBA8;
        if (textureArrays[i]->format == GL_RGBA)
        {
            sizedFormat = GL_RGBA8;
        }
        else if (textureArrays[i]->format == GL_RGB)
        {
            sizedFormat = GL_RGB8;
        }

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 3, sizedFormat, textureArrays[i]->width, textureArrays[i]->height, textureArrays[i]->depth);
        //glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, sizedFormat, textureArrays[i]->width, textureArrays[i]->height, textureArrays[i]->depth);
        unsigned depth = 0;
        for (auto& resource : resources)
        {
            if (EqualLayout(*textureArrays[i], *resource.first))
            {
                resource.second->texIndex = i;
                resource.second->layerIndex = depth;

                //unsigned size = resource.first->width * resource.first->height * resource.first->bpp;
                //byte* new_array = new byte[size];

                //glEnable(GL_TEXTURE_2D);
                //glBindTexture(GL_TEXTURE_2D, resource.first->id);
                //glGetTexImage(GL_TEXTURE_2D, 0, textureArray->format, GL_UNSIGNED_BYTE, (void*)(new_array));
                //glBindTexture(GL_TEXTURE_2D, 0);

                //glGetTextureImage(resource.first->id, 0, textureArray->format, GL_UNSIGNED_BYTE, size, new_array);

                //ImGui::Image(&new_array[0], ImVec2(100, 100));
                //ImGui::Image((void*)resource.first->id, ImVec2(100, 100));

                std::string extension = resource.first->path.substr(resource.first->path.find_last_of(".") + 1);
                unsigned imgId = ModuleTexture::LoadImg(resource.first->path.c_str(), extension != "tif");

                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY, // target
                    0, // level
                    0, // xoffset
                    0, // yoffset
                    depth, // zoffset
                    textureArrays[i]->width, // width
                    textureArrays[i]->height, // height
                    1, // depth
                    textureArrays[i]->format, // format
                    GL_UNSIGNED_BYTE, // type
                    ModuleTexture::GetData() // texture_data
                );

                ModuleTexture::DeleteImg(imgId);
                //delete[] new_array;
                ++depth;
            }
        }

        // Array texture parameters
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 2);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
}

void Hachiko::TextureBatch::GenerateMaterials(const std::vector<const ComponentMesh*>& components)
{
    materials.resize(components.size());

    for (unsigned i = 0; i < components.size(); ++i)
    {
        const ComponentMaterial* material_comp = components[i]->GetGameObject()->GetComponent<ComponentMaterial>();

        const ResourceMaterial* material = material_comp->GetMaterial();

        materials[i].diffuseColor = material->diffuse_color;
        materials[i].specularColor = material->specular_color;
        materials[i].shininess = material->shininess;
        materials[i].hasDiffuseMap = material->HasDiffuse();
        materials[i].hasSpecularMap = material->HasSpecular();
        materials[i].hasNormalMap = material->HasNormal();
        if (materials[i].hasDiffuseMap)
        {
            materials[i].diffuseMap = (*resources[material->diffuse]);
            //materials[i].diffuseMap.texIndex = resources[&material->diffuse]->texIndex;
            //materials[i].diffuseMap.layerIndex = resources[&material->diffuse]->layerIndex;
        }
        if (materials[i].hasSpecularMap)
        {
            materials[i].specularMap = (*resources[material->specular]);
            //materials[i].specularMap.texIndex = resources[&material->specular]->texIndex;
            //materials[i].specularMap.layerIndex = resources[&material->specular]->layerIndex;
        }
        if (materials[i].hasNormalMap)
        {
            materials[i].normalMap = (*resources[material->normal]);
            //materials[i].specularMap.texIndex = resources[&material->specular]->texIndex;
            //materials[i].specularMap.layerIndex = resources[&material->specular]->layerIndex;
        }
        //materials[i].normalMap;
        //materials[i].padding0;
        //materials[i].padding1;
    }
}

void Hachiko::TextureBatch::UpdateTextureBatch() 
{
    for (unsigned i = 0; i < textureArrays.size(); ++i)
    {
        //glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrays[i]->id);
        glUniform1i(1 + i, i);
    }
}

void Hachiko::TextureBatch::UpdateMaterials(unsigned ssbo_id)
{
    if (materials.size() <= 0)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    else
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Material) * materials.size(), &materials[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}

void Hachiko::TextureBatch::ImGuiWindow()
{
    ImGui::Text("TEXTURE_BATCH");

    for (unsigned i = 0; i < textureArrays.size(); ++i)
    {
        ImGui::BulletText("TextureArray ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(i).c_str());

        ImGui::Text("Depth (number of textures): ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(textureArrays[i]->depth).c_str());

        ImGui::Text("Width: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(textureArrays[i]->width).c_str());

        ImGui::Text("Height: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(textureArrays[i]->height).c_str());

        ImGui::Text("Format: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(textureArrays[i]->format).c_str());

        for (auto& resource : resources)
        {
            if (EqualLayout(*textureArrays[i], *resource.first))
            {
                ImGui::Text(resource.first->path.c_str());
            }
        }
    }
}


bool Hachiko::TextureBatch::EqualLayout(const TextureArray& texuteLayout, const ResourceTexture& texture)
{
    return (texuteLayout.width == texture.width && texuteLayout.height == texture.height && texuteLayout.format == texture.format);
}
