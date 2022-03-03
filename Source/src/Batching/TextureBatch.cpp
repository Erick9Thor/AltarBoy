#include "core/hepch.h"
#include "TextureBatch.h"

#include "Core/GameObject.h"
#include "Components/ComponentMesh.h"
#include "Components/ComponentMaterial.h"

#include "Modules/ModuleProgram.h"

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

void Hachiko::TextureBatch::AddTexture(const Texture* texture) 
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

void Hachiko::TextureBatch::AddMaterial(const ComponentMaterial* material) 
{
    const ResourceMaterial* resource_material = material->GetMaterial();

    if (material->use_diffuse_texture)
    {
        AddTexture(&resource_material->diffuse);
    }
    if (material->use_specular_texture)
    {
        AddTexture(&resource_material->specular);
    }
}

void Hachiko::TextureBatch::GenerateBatch() 
{
    // TODO: improve performance (for inside for)

    int maxLayers; // maximum number of array texture layers 
    int maxUnits; // maximum number of texture units
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);

    // TODO: security checks with these maximums

    unsigned index = 0;
    for (TextureArray* textureArray : textureArrays)
    {
        glGenTextures(1, &textureArray->id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray->id);

        // Generate texture array
        //glTexStorage3D(GL_TEXTURE_2D_ARRAY, 3, textureArray->format, textureArray->width, textureArray->height, textureArray->depth);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, textureArray->format, textureArray->width, textureArray->height, textureArray->depth);
        unsigned depth = 0;
        for (auto& resource : resources)
        {
            if (EqualLayout(*textureArray, *resource.first))
            {
                resource.second->texIndex = textureArray->id;
                resource.second->layerIndex = depth;

                byte* new_array = new byte[resource.first->width * resource.first->height * resource.first->bpp];

                glGetTexImage(GL_TEXTURE_2D, 0, textureArray->format, GL_UNSIGNED_BYTE, (void*)(new_array));

                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY, // target
                    0, // level
                    0, // xoffset
                    0, // yoffset
                    depth, // zoffset
                    textureArray->width, // width
                    textureArray->height, // height
                    1, // depth
                    textureArray->format, // format
                    GL_UNSIGNED_BYTE, // type
                    new_array // texture_data
                );

                delete[] new_array;
                ++depth;
            }
        }

        // Array texture parameters
        //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
        //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 2);
        //glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
        materials[i].specularColor = material->diffuse_color;
        materials[i].shininess = material->shininess;
        materials[i].hasDiffuseMap = material_comp->use_diffuse_texture;
        materials[i].hasSpecularMap = material_comp->use_specular_texture;
        materials[i].hasNormalMap = 0;
        if (materials[i].hasDiffuseMap)
        {
            materials[i].diffuseMap = (*resources[&material->diffuse]);
        }
        if (materials[i].hasSpecularMap)
        {
            materials[i].specularMap = (*resources[&material->specular]);
        }
        materials[i].normalMap;
        //materials[i].padding0;
        //materials[i].padding1;
    }
}

void Hachiko::TextureBatch::UpdateTextureBatch() 
{
    std::vector<int> texture_slots;

    for (unsigned i = 0; i < textureArrays.size(); ++i)
    {
        //glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrays[i]->id);
        texture_slots.push_back(i);
    }
    
    Program* main_program = App->program->GetMainProgram();
    main_program->BindUniformInts("allMyTextures", texture_slots.size(), &texture_slots[0]);
}

void Hachiko::TextureBatch::UpdateMaterials(unsigned ssbo_id)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(materials), &materials[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Hachiko::TextureBatch::ImGuiWindow()
{
    if (ImGui::Begin("TextureBatch"))
    {
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
    ImGui::End();
}


bool Hachiko::TextureBatch::EqualLayout(const TextureArray& texuteLayout, const Texture& texture)
{
    return (texuteLayout.width == texture.width && texuteLayout.height == texture.height && texuteLayout.format == texture.format);
}