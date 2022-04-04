#include "core/hepch.h"
#include "ResourceMaterial.h"
#include "modules/ModuleResources.h"

Hachiko::ResourceMaterial::ResourceMaterial(UID uid) :
    Resource(uid, Type::MATERIAL) {}

Hachiko::ResourceMaterial::~ResourceMaterial()
{}

void Hachiko::ResourceMaterial::DrawGui() 
{
    ImGui::Text(name.c_str());
    ImGui::DragFloat("Shininess", &shininess);

    // Diffuse
    ImGui::Text("Diffuse: ");
    ImGui::SameLine();
    ImGui::Text((diffuse != nullptr) ? diffuse->path.c_str() : "None");
    if (diffuse != nullptr)
    {
        diffuse->DrawGui();
        RemoveTexture(ResourceTexture::Type::DIFFUSE);
    }
    else
    {
        ImGui::ColorEdit4("Diffuse color", &diffuse_color[0]);
        AddTexture(ResourceTexture::Type::DIFFUSE);
    }

    // Specular

    // TODO: texture selection
    ImGui::Text("Specular: ");
    ImGui::SameLine();
    ImGui::Text((specular != nullptr) ? specular->path.c_str() : "None");
    if (specular != nullptr)
    {
        specular->DrawGui();
        RemoveTexture(ResourceTexture::Type::SPECULAR);
    }
    else
    {
        ImGui::ColorEdit4("Specular color", &specular_color[0]);
        AddTexture(ResourceTexture::Type::SPECULAR);
    }

    // Normals
    ImGui::Text("Normals: ");
    ImGui::SameLine();
    ImGui::Text((normal != nullptr) ? normal->path.c_str() : "None");
    if (normal != nullptr)
    {
        normal->DrawGui();
        RemoveTexture(ResourceTexture::Type::NORMALS);
    }
    else
    {
        AddTexture(ResourceTexture::Type::NORMALS);
    }
}

void Hachiko::ResourceMaterial::AddTexture(ResourceTexture::Type type)
{
    const std::string title = StringUtils::Concat("Select texture ", TypeToString(type));
    ResourceTexture* res = nullptr;
    //ImGui::PushID((int)type);
    //const int index = TexturesTypesListBox();
    //ImGui::PopID();

    if (ImGui::Button(StringUtils::Concat(TypeToString(type).c_str(), " Texture").c_str()))
    {
        ImGuiFileDialog::Instance()->OpenDialog(
            title.c_str(),
            "Select Texture",
            ".*",
            "./library/textures/",
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
            const std::filesystem::path texture_path = ImGuiFileDialog::Instance()->GetFilePathName().c_str();
                        
            res = App->resources->GetTexture(texture_path.filename().replace_extension().string().c_str());
        }

        ImGuiFileDialog::Instance()->Close();
    }

    SetTexture(res, type);
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
        }
    }
}
