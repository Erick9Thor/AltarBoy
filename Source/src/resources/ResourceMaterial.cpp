#include "core/hepch.h"
#include "ResourceMaterial.h"
#include "modules/ModuleResources.h"

Hachiko::ResourceMaterial::ResourceMaterial(UID uid) :
    Resource(uid, Type::MATERIAL) {}

Hachiko::ResourceMaterial::~ResourceMaterial()
{}

void Hachiko::ResourceMaterial::DrawGui() 
{
    //ImGui::InputText("Name", &name[0], 64);
    ImGui::Text(name.c_str());

    ImGui::DragFloat("Shininess", &shininess);

    // Diffuse

    // TODO: texture selection
    ImGui::Text("Diffuse: ");
    ImGui::SameLine();
    ImGui::Text((diffuse != nullptr) ? diffuse->path.c_str() : "None");
    if (diffuse != nullptr)
    {
        diffuse->DrawGui();
    }
    else
    {
        ImGui::ColorEdit4("Diffuse color", &diffuse_color[0]);
        if (ImGui::Button("Add Diffuse Texture"))
        {
            //SetTexture(App->resources->GetTexture(), ResourceTexture::Type::DIFFUSE);
        }
    }

    // Specular

    // TODO: texture selection
    ImGui::Text("Specular: ");
    ImGui::SameLine();
    ImGui::Text((specular != nullptr) ? specular->path.c_str() : "None");
    if (specular != nullptr)
    {
        specular->DrawGui();
    }
    else
    {
        ImGui::ColorEdit4("Specular color", &specular_color[0]);
        if (ImGui::Button("Add Diffuse Texture"))
        {
            //SetTexture(App->resources->GetTexture(), ResourceTexture::Type::SPECULAR);
        }
    }

    // Normals

    // TODO: texture selection
    ImGui::Text("Normals: ");
    ImGui::SameLine();
    ImGui::Text((normal != nullptr) ? normal->path.c_str() : "None");
    if (normal != nullptr)
    {
        normal->DrawGui();
    }
    else
    {
        if (ImGui::Button("Add Normals Texture"))
        {
            //SetTexture(App->resources->GetTexture(), ResourceTexture::Type::NORMALS);
        }
    }
}
