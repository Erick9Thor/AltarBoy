#include "ComponentMaterial.h"

#include "UI/ImGuiUtils.h"

#include "Application.h"
#include "Core/Scene.h"
#include "Modules/ModuleTexture.h"

#include "Core/GameObject.h"

#include "Importers/MaterialImporter.h"

#include <imgui.h>

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
                    ImGui::Text("Diffuse: %dx%d %s", diffuse.width, diffuse.height, diffuse.path.c_str());
                    ImGui::Image(reinterpret_cast<void*>(diffuse.id), ImVec2(150, 150));
                }
            }
            if (!use_diffuse_texture)
                ImGuiUtils::CompactColorPicker("Diffuse Color", &material->diffuse_color[0]);

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
                ImGuiUtils::CompactColorPicker("Specular Color", &material->specular_color[0]);
        }
        else
        {
            ImGui::Text("No material resource");
        }
    }
    ImGui::PopID();
}
