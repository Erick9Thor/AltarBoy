#include "core/hepch.h"

#include "ComponentMaterial.h"

#include "modules/ModuleTexture.h"
#include "importers/MaterialImporter.h"

Hachiko::ComponentMaterial::ComponentMaterial(GameObject* conatiner) :
    Component(Type::MATERIAL, conatiner) {}

Hachiko::ComponentMaterial::~ComponentMaterial()
{
}

void Hachiko::ComponentMaterial::Import(aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name)
{
    //material = MaterialImporter::Material::Import(assimp_material, model_path, model_name);
    //use_diffuse_texture = material->diffuse.loaded;
    //use_specular_texture = material->specular.loaded;
}
// TODO: Move to serializers
//void Hachiko::ComponentMaterial::Save(JsonFormatterValue j_component) const
//{
//    //MaterialImporter::Material::Save(material, j_component);
//}
//
//void Hachiko::ComponentMaterial::Load(JsonFormatterValue j_component)
//{
//    //material = MaterialImporter::Material::Load(j_component);
//    //use_diffuse_texture = material->diffuse.loaded;
//    //use_specular_texture = material->specular.loaded;
//}

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

    /*
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
