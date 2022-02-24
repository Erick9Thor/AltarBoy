#include "core/hepch.h"

#include "ComponentMaterial.h"

#include "modules/ModuleTexture.h"
#include "importers/MaterialImporter.h"

Hachiko::ComponentMaterial::ComponentMaterial(GameObject* conatiner) :
    Component(Type::MATERIAL, conatiner) {}

Hachiko::ComponentMaterial::~ComponentMaterial()
{
    RELEASE(material);
}

void Hachiko::ComponentMaterial::Import(aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name)
{
    material = MaterialImporter::Material::Import(assimp_material, model_path, model_name);
    use_diffuse_texture = material->diffuse.loaded;
    use_specular_texture = material->specular.loaded;
}

void Hachiko::ComponentMaterial::Save(JsonFormatterValue j_component) const
{
    MaterialImporter::Material::Save(material, j_component);
}

void Hachiko::ComponentMaterial::Load(JsonFormatterValue j_component)
{
    material = MaterialImporter::Material::Load(j_component);
    use_diffuse_texture = material->diffuse.loaded;
    use_specular_texture = material->specular.loaded;
}

void Hachiko::ComponentMaterial::DrawGui()
{
    static ImGuiTreeNodeFlags texture_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Material"))
    {
        if (material)
        {
            const Texture& diffuse = material->diffuse;
            if (diffuse.loaded)
            {
                ImGui::Checkbox("Diff Texture", &use_diffuse_texture);
                if (use_diffuse_texture)
                {
                    if (ImGui::TreeNodeEx((void*)&diffuse, texture_flags, "Diffuse"))
                    {
                        ImGui::Image(reinterpret_cast<void*>(diffuse.id), ImVec2(80, 80));
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                        ImGui::Text("%dx%d", diffuse.width, diffuse.height);
                        ImGui::Text("Path: %s", diffuse.path.c_str());
                        ImGui::EndGroup();
                        ImGui::TreePop();
                    }
                }
            }
            if (!use_diffuse_texture)
                ImGui::ColorEdit4("Diffuse Color", &material->diffuse_color[0]);

            const Texture& specular = material->specular;
            ImGui::InputFloat("Shininess", &material->shininess);
            if (specular.loaded)
            {
                ImGui::Checkbox("Spec Texture", &use_specular_texture);
                if (use_specular_texture)
                {
                    ImGui::Text("Specular: %dx%d %s", specular.width, specular.height, specular.path.c_str());
                    ImGui::Image(reinterpret_cast<void*>(specular.id), ImVec2(150, 150));
                }
            }
            if (!use_specular_texture)
                ImGui::ColorEdit4("Specular Color", &material->specular_color[0]);
        }
        else
        {
            ImGui::Text("No material resource");
        }
    }
    ImGui::PopID();
}
