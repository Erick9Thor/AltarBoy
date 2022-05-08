#include "core/hepch.h"
#include "TextureBatch.h"

#include "core/GameObject.h"
#include "components/ComponentMeshRenderer.h"
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

    for (TextureArray* textureArray : texture_arrays)
    {
        glDeleteTextures(textureArray->depth, &textureArray->id);
        delete textureArray;
    }
    texture_arrays.clear();
}

void Hachiko::TextureBatch::AddMaterial(const ResourceMaterial* resource_material)
{
    if (resource_material->HasDiffuse())
    {
        AddTexture(resource_material->diffuse);
    }
    if (resource_material->HasSpecular())
    {
        AddTexture(resource_material->specular);
    }
    if (resource_material->HasNormal())
    {
        AddTexture(resource_material->normal);
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
        for (TextureArray* textureArray : texture_arrays)
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

            texture_arrays.push_back(textureArray);
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
    if (texture_arrays.size() > maxUnits - 8)
    {
        HE_LOG("[Error] TextureBatch: there are more texture units than the maximum.");
    }

    unsigned index = 0;
    for (unsigned i = 0; i < texture_arrays.size(); ++i)
    {
        // Security check, amount of layers in a texture array
        if (texture_arrays[i]->depth > maxLayers)
        {
            HE_LOG("[Error] TextureBatch: there are layers in a texture array than the maximum.");
        }

        glGenTextures(1, &texture_arrays[i]->id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture_arrays[i]->id);

        // Generate texture array
        unsigned sizedFormat = GL_RGBA8;
        if (texture_arrays[i]->format == GL_RGBA)
        {
            sizedFormat = GL_RGBA8;
        }
        else if (texture_arrays[i]->format == GL_RGB)
        {
            sizedFormat = GL_RGB8;
        }

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 3, sizedFormat, texture_arrays[i]->width, texture_arrays[i]->height, texture_arrays[i]->depth);
        //glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, sizedFormat, textureArrays[i]->width, textureArrays[i]->height, textureArrays[i]->depth);
        unsigned depth = 0;
        for (auto& resource : resources)
        {
            if (EqualLayout(*texture_arrays[i], *resource.first))
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
                    texture_arrays[i]->width, // width
                    texture_arrays[i]->height, // height
                    1, // depth
                    texture_arrays[i]->format, // format
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

void Hachiko::TextureBatch::Draw(const std::vector<const ComponentMeshRenderer*>& components)
{
    GenerateMaterials(components);
    UpdateTextureBatch();
    UpdateMaterials();
}

void Hachiko::TextureBatch::GenerateMaterials(const std::vector<const ComponentMeshRenderer*>& components)
{
    materials.resize(components.size());

    for (unsigned i = 0; i < components.size(); ++i)
    {
        const ResourceMaterial* material = components[i]->GetMaterial();

        materials[i].diffuse_color = material->diffuse_color;
        materials[i].specular_color = material->specular_color;
        materials[i].diffuse_flag = material->HasDiffuse();
        materials[i].specular_flag = material->HasSpecular();
        materials[i].normal_flag = material->HasNormal();
        materials[i].metallic_flag = material->HasMetalness();

        if (materials[i].diffuse_flag)
        {
            materials[i].diffuse_map = (*resources[material->diffuse]);
        }
        if (materials[i].specular_flag)
        {
            materials[i].specular_map = (*resources[material->specular]);
        }
        if (materials[i].normal_flag)
        {
            materials[i].normal_map = (*resources[material->normal]);
        }
        if (materials[i].metallic_flag)
        {
            materials[i].metallic_map = (*resources[material->metalness]);
        }

        materials[i].smoothness = material->smoothness;
        materials[i].metalness_value = material->metalness_value;
        materials[i].is_metallic = material->is_metallic;
        materials[i].smoothness_alpha = material->smoothness_alpha;
        materials[i].is_transparent = material->is_transparent;
    }
}

void Hachiko::TextureBatch::UpdateTextureBatch() 
{
    for (unsigned i = 0; i < texture_arrays.size(); ++i)
    {
        //glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture_arrays[i]->id);
        glUniform1i(1 + i, i);
    }
}

void Hachiko::TextureBatch::UpdateMaterials()
{
    App->program->UpdateMaterials(materials);
}

void Hachiko::TextureBatch::ImGuiWindow()
{
    ImGui::Text("TEXTURE_BATCH");

    for (unsigned i = 0; i < texture_arrays.size(); ++i)
    {
        ImGui::BulletText("TextureArray ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(i).c_str());

        ImGui::Text("Depth (number of textures): ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(texture_arrays[i]->depth).c_str());

        ImGui::Text("Width: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(texture_arrays[i]->width).c_str());

        ImGui::Text("Height: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(texture_arrays[i]->height).c_str());

        ImGui::Text("Format: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(texture_arrays[i]->format).c_str());

        for (auto& resource : resources)
        {
            if (EqualLayout(*texture_arrays[i], *resource.first))
            {
                ImGui::Text(resource.first->path.c_str());
            }
        }
    }
}


bool Hachiko::TextureBatch::EqualLayout(const TextureArray& texture_layout, const ResourceTexture& texture)
{
    return (texture_layout.width == texture.width && texture_layout.height == texture.height && texture_layout.format == texture.format);
}
