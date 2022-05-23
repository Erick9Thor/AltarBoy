#include "core/hepch.h"
#include "ResourceMaterial.h"
#include "modules/ModuleResources.h"
#include "importers/MaterialImporter.h"

Hachiko::ResourceMaterial::ResourceMaterial(UID uid) :
    Resource(uid, Type::MATERIAL) {}

Hachiko::ResourceMaterial::~ResourceMaterial()
{}

void Hachiko::ResourceMaterial::DrawGui() 
{
    static const ImGuiTreeNodeFlags texture_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    ImGui::Text("Material: %s", name.c_str());

    ImGuiUtils::Combo("Material Type", material_types, is_metallic);

    ImGuiUtils::Combo("###", transparency, is_transparent);

    if (ImGui::TreeNodeEx((void*)&diffuse, texture_flags, "Diffuse"))
    {
        if (diffuse != nullptr)
        {
            ImGui::Image(reinterpret_cast<void*>(diffuse->GetId()), ImVec2(80, 80));
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text("%dx%d", diffuse->width, diffuse->height);
            ImGui::Text("Path: %s", diffuse->path.c_str());

            // TODO: textue configuration (maybe delegate to the ResourceTexture)

            RemoveTexture(ResourceTexture::Type::DIFFUSE);

            ImGui::EndGroup();
        }
        else
        {
            ImGui::ColorEdit4("Diffuse color", &diffuse_color[0]);
            AddTexture(ResourceTexture::Type::DIFFUSE);
        }
        ImGui::TreePop();
    }
    if (is_metallic)
    {
        if (ImGui::TreeNodeEx((void*)&metalness, texture_flags, "Metalness"))
        {
            if (metalness != nullptr)
            {
                ImGui::Image(reinterpret_cast<void*>(metalness->GetId()), ImVec2(80, 80));
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::Text("%dx%d", metalness->width, metalness->height);
                ImGui::Text("Path: %s", metalness->path.c_str());

                RemoveTexture(ResourceTexture::Type::METALNESS);

                ImGui::EndGroup();
            }
            else
            {
                ImGui::SliderFloat("Metalness", &metalness_value, 0.0f, 1.0f, "%.2f", 1.0f);
                AddTexture(ResourceTexture::Type::METALNESS);
            }
            ImGui::SliderFloat("Smoothness", &smoothness, 0.0f, 1.0f, "%.2f", 1.0f);

            alpha_channels[1] = material_types[is_metallic];
            ImGuiUtils::Combo("Alpha", alpha_channels, smoothness_alpha);

            ImGui::TreePop();
        }
    }
    else 
    {
        if (ImGui::TreeNodeEx((void*)&specular, texture_flags, "Specular"))
        {
            if (specular != nullptr)
            {
                ImGui::Image(reinterpret_cast<void*>(specular->GetId()), ImVec2(80, 80));
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::Text("%dx%d", specular->width, specular->height);
                ImGui::Text("Path: %s", specular->path.c_str());

                // TODO: textue configuration (maybe delegate to the ResourceTexture)

                RemoveTexture(ResourceTexture::Type::SPECULAR);

                ImGui::EndGroup();
            }
            else
            {
                ImGui::ColorEdit4("Specular color", &specular_color[0]);
                AddTexture(ResourceTexture::Type::SPECULAR);
            }
            ImGui::SliderFloat("Smoothness", &smoothness, 0.0f, 1.0f, "%.2f", 1.0f);

            alpha_channels[1] = material_types[is_metallic];
            ImGuiUtils::Combo("Alpha", alpha_channels, smoothness_alpha);

            ImGui::TreePop();
        }
    }
    if (ImGui::TreeNodeEx((void*)&normal, texture_flags, "Normal"))
    {
        if (normal != nullptr)
        {
            ImGui::Image(reinterpret_cast<void*>(normal->GetId()), ImVec2(80, 80));
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text("%dx%d", normal->width, normal->height);
            ImGui::Text("Path: %s", normal->path.c_str());

            // TODO: textue configuration (maybe delegate to the ResourceTexture)

            RemoveTexture(ResourceTexture::Type::NORMALS);

            ImGui::EndGroup();
        }
        else
        {
            AddTexture(ResourceTexture::Type::NORMALS);
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx((void*)&emissive, texture_flags, "Emissive"))
    {
        if (emissive != nullptr)
        {
            ImGui::Image(reinterpret_cast<void*>(emissive->GetId()), ImVec2(80, 80));
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text("%dx%d", emissive->width, emissive->height);
            ImGui::Text("Path: %s", emissive->path.c_str());

            // TODO: textue configuration (maybe delegate to the ResourceTexture)

            ImGui::ColorEdit4("Emissive color", &emissive_color[0]);
            RemoveTexture(ResourceTexture::Type::EMISSIVE);

            ImGui::EndGroup();
        }
        else
        {
            ImGui::ColorEdit4("Emissive color", &emissive_color[0]);
            AddTexture(ResourceTexture::Type::EMISSIVE);
        }
        ImGui::TreePop();
    }
    if (ImGui::SmallButton("Save"))
    {
        UpdateMaterial();
    }
}



void Hachiko::ResourceMaterial::AddTexture(ResourceTexture::Type type)
{
    const std::string title = StringUtils::Concat("Select texture ", TypeToString(type)) + "##" + this->name;
    ResourceTexture* res = nullptr;

    if (ImGui::Button(StringUtils::Concat(TypeToString(type).c_str(), " Texture").c_str()))
    {
        ImGuiFileDialog::Instance()->OpenDialog(
            title.c_str(),
            "Select Texture",
            ".png,.tif,.jpg,.tga",
            "./assets/textures/",
            1,
            nullptr,
              ImGuiFileDialogFlags_DontShowHiddenFiles 
            | ImGuiFileDialogFlags_DisableCreateDirectoryButton 
            | ImGuiFileDialogFlags_HideColumnType
            | ImGuiFileDialogFlags_HideColumnDate);
    }
    
    if (ImGuiFileDialog::Instance()->Display(title.c_str()))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string texture_path = ImGuiFileDialog::Instance()->GetFilePathName();
            texture_path.append(META_EXTENSION);
            YAML::Node texture_node = YAML::LoadFile(texture_path);

            res = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, 
                texture_node[RESOURCES][0][RESOURCE_ID].as<UID>()));
        }

        ImGuiFileDialog::Instance()->Close();
    }
    if (res != nullptr)
    {
        SetTexture(res, type);
        UpdateMaterial();
    }
}

void Hachiko::ResourceMaterial::RemoveTexture(ResourceTexture::Type type)
{
    if (ImGui::Button(StringUtils::Concat("Remove " , TypeToString(type).c_str()).c_str()))
    {
        switch (type)
        {
        case ResourceTexture::Type::DIFFUSE:
            diffuse = nullptr;
            break;
        case ResourceTexture::Type::SPECULAR:
            specular = nullptr;
            break;
        case ResourceTexture::Type::NORMALS:
            normal = nullptr;
            break;
        case ResourceTexture::Type::METALNESS:
            metalness = nullptr;
            break;
        case ResourceTexture::Type::EMISSIVE:
            emissive = nullptr;
            break;
        }
    }

    UpdateMaterial();
}

void Hachiko::ResourceMaterial::UpdateMaterial()
{
    MaterialImporter material_importer;
    material_importer.Save(GetID(), this);
}
