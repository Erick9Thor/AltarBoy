#include "core/hepch.h"
#include "ResourceMaterial.h"

Hachiko::ResourceMaterial::ResourceMaterial(UID uid) :
    Resource(uid, Type::MATERIAL) {}

Hachiko::ResourceMaterial::~ResourceMaterial()
{}

void Hachiko::ResourceMaterial::DrawGui() 
{
    ImGui::InputText("Name", &name[0], 64);

    ImGui::DragFloat("Shininess", &shininess);

    // Diffuse

    // TODO: texture selection
    ImGui::Text((diffuse != nullptr) ? diffuse->path.c_str() : "None");
    if (diffuse != nullptr)
    {
        diffuse->DrawGui();
    }
    else
    {
        ImGui::ColorEdit4("Diffuse color", &diffuse_color[0]);
    }

    // Specular

    // TODO: texture selection
    ImGui::Text((specular != nullptr) ? specular->path.c_str() : "None");
    if (specular != nullptr)
    {
        specular->DrawGui();
    }
    else
    {
        ImGui::ColorEdit4("Specular color", &specular_color[0]);
    }

    // Normals

    // TODO: texture selection
    ImGui::Text((normals != nullptr) ? normals->path.c_str() : "None");
    if (normals != nullptr)
    {
        normals->DrawGui();
    }
}
