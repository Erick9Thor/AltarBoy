#include "core/hepch.h"

#include "ComponentMaterial.h"

#include "modules/ModuleTexture.h"
#include "modules/ModuleResources.h"
#include "importers/MaterialImporter.h"

Hachiko::ComponentMaterial::ComponentMaterial(GameObject* container) 
    : Component(Type::MATERIAL, container) 
{
    if (container->scene_owner)
    {
        container->scene_owner->OnMeshesChanged();
    }
}

Hachiko::ComponentMaterial::~ComponentMaterial() 
{
    if (game_object->scene_owner)
    {
        game_object->scene_owner->OnMeshesChanged();
    }
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
    LoadMaterial(node[COMPONENT_ID].as<UID>());
    material->SetName(node[MATERIAL_NAME].as<std::string>());
}

void Hachiko::ComponentMaterial::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Material"))
    {
        ChangeMaterial();
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
}

void Hachiko::ComponentMaterial::LoadMaterial(UID id)
{
    material = static_cast<ResourceMaterial*> (App->resources->GetResource(Resource::Type::MATERIAL, id));
}

void Hachiko::ComponentMaterial::ChangeMaterial()
{
    const std::string title = "Select material";
    ResourceMaterial* res = nullptr;

    if (ImGui::Button("Select material"))
    {
        ImGuiFileDialog::Instance()->OpenDialog(title.c_str(),
                                                "Select Material",
                                                ".mat",
                                                "./assets/materials/",
                                                1,
                                                nullptr,
                                                ImGuiFileDialogFlags_DontShowHiddenFiles | ImGuiFileDialogFlags_DisableCreateDirectoryButton | ImGuiFileDialogFlags_HideColumnType
                                                    | ImGuiFileDialogFlags_HideColumnDate);
    }

    if (ImGuiFileDialog::Instance()->Display(title.c_str()))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string material_path = ImGuiFileDialog::Instance()->GetFilePathName();
            material_path.append(META_EXTENSION);
            YAML::Node material_node = YAML::LoadFile(material_path);

            res = static_cast<ResourceMaterial*>(App->resources->GetResource(Resource::Type::MATERIAL, material_node[GENERAL_NODE][GENERAL_ID].as<UID>()));
        }

        ImGuiFileDialog::Instance()->Close();
    }
    if (res != nullptr)
    {
        // Unload material
        material = res;
    }
}