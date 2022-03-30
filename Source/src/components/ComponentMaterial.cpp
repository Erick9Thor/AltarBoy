#include "core/hepch.h"

#include "ComponentMaterial.h"

#include "modules/ModuleTexture.h"
#include "modules/ModuleResources.h"

#include "resources/ResourceMaterial.h"
#include "importers/MaterialImporter.h"

Hachiko::ComponentMaterial::ComponentMaterial(GameObject* container) 
    : Component(Type::MATERIAL, container) 
{
}

Hachiko::ComponentMaterial::~ComponentMaterial()
{
}

void Hachiko::ComponentMaterial::Save(YAML::Node& node) const
{
    if (material == nullptr)
    {
        return;
    }

    node[MATERIAL_NAME] = material->GetName();
}

void Hachiko::ComponentMaterial::Load(const YAML::Node& node)
{
    SetID(node[COMPONENT_ID].as<UID>());
    LoadMaterial(node[MATERIAL_NAME].as<std::string>());
    material->SetName(node[MATERIAL_NAME].as<std::string>());
}

void Hachiko::ComponentMaterial::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Material"))
    {
        if (material != nullptr)
        {
            material->DrawGui();
        }
        else
        {
            ImGui::Text("No material resource");
            // TODO: material selection option
        }
    }
    ImGui::PopID();

    /* TODO: Delete this comented code fi not necessary
    static const ImGuiTreeNodeFlags texture_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Material"))
    {
        if (material)
        {
            ImGui::InputText("Name", &material->name[0], 64);

            const Texture& diffuse = material->diffuse;

            if (ImGui::TreeNodeEx((void*)&diffuse, texture_flags, "Diffuse"))
            {
                if (diffuse.loaded)
                {
                    ImGui::Checkbox("Diff Texture", &use_diffuse_texture);
                    if (use_diffuse_texture)
                    {
                        ImGui::Image(reinterpret_cast<void*>(diffuse.id), ImVec2(80, 80));
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                        ImGui::Text("%dx%d", diffuse.width, diffuse.height);
                        ImGui::Text("Path: %s", diffuse.path.c_str());

                        ImGui::Button("Change Texture");
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetTooltip("Currently not working, waiting for Texture Resource");
                        }

                        ImGui::Button("Detach Texture");
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetTooltip("Currently not working, waiting for Texture Resource");
                        }
                        ImGui::EndGroup();
                    }
                }

                if (!use_diffuse_texture)
                {
                    ImGui::DragFloat4("Diffuse", (float*)&material->diffuse_color, 0.005f, 0.0f, +FLT_MAX, "%.3f", 1.0f);
                }
                ImGui::TreePop();
            }

            const Texture& specular = material->specular;
            if (ImGui::TreeNodeEx((void*)&specular, texture_flags, "Specular"))
            {
                if (specular.loaded)
                {
                    ImGui::Checkbox("Spec Texture", &use_specular_texture);
                    if (use_specular_texture)
                    {
                        ImGui::Image(reinterpret_cast<void*>(specular.id), ImVec2(80, 80));
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                        ImGui::Text("%dx%d", specular.width, specular.height);
                        ImGui::Text("Path: %s", specular.path.c_str());
                        ImGui::Button("Change Texture");
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetTooltip("Currently not working, waiting for Texture Resource");
                        }
                        ImGui::Button("Detach Texture");
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetTooltip("Currently not working, waiting for Texture Resource");
                        }
                        ImGui::EndGroup();
                    }
                }

                if (!use_specular_texture)
                {
                    ImGui::DragFloat4("Specular", (float*)&material->specular_color, 0.005f, 0.0f, +FLT_MAX, "%.3f", 1.0f);
                }
                ImGui::TreePop();
            }
            
            ImGui::DragFloat("Shininess", &material->shininess, 0.05, 0.0f, +FLT_MAX, "%.3f", 1.0f);
            ImGui::Text("");
        }
        else
        {
            ImGui::Text("No material resource");
        }
    }
    ImGui::PopID();
    */
}

void Hachiko::ComponentMaterial::LoadMaterial(const std::string& material_path)
{
    material = App->resources->GetMaterial(material_path);
}
