#include "core/hepch.h"

#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "modules/ModuleProgram.h"
#include "importers/MeshImporter.h"
#include "resources/ResourceMesh.h"

Hachiko::ComponentMesh::ComponentMesh(GameObject* container) : Component(Type::MESH, container) {}

Hachiko::ComponentMesh::ComponentMesh(GameObject* container, ResourceMesh* res) 
    : Component(Type::MESH, container)
{
    meshes.push_back(res);
}

Hachiko::ComponentMesh::~ComponentMesh()
{
    for (auto mesh : meshes)
    {
        RELEASE(mesh);
    }
}

void Hachiko::ComponentMesh::Draw(ComponentCamera* camera, Program* program)
{
    if (meshes.empty())
    {
        return;
    }
    // TODO: Why we take care of other components?
    program->BindUniformFloat4x4("model", game_object->GetTransform()->GetMatrix().ptr());

    const ComponentMaterial* material = game_object->GetComponent<ComponentMaterial>();
    App->program->UpdateMaterial(material);

    for (auto mesh : meshes)
    {
        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)], GL_UNSIGNED_INT, nullptr);
    }
}

void Hachiko::ComponentMesh::DrawStencil(ComponentCamera* camera, Program* program) const
{
    if (meshes.empty())
    {
        return;
    }
    program->BindUniformFloat4x4("model", game_object->GetTransform()->GetMatrix().ptr());

    for (auto mesh : meshes)
    {
        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)], GL_UNSIGNED_INT, nullptr);
    }
}

void Hachiko::ComponentMesh::LoadMesh(const char* mesh_path)
{
    MeshImporter mesh_importer;
    std::filesystem::path m_path(mesh_path);
    LoadMesh(Hachiko::UUID::StringToUID(m_path.filename().replace_extension().string()));
}

void Hachiko::ComponentMesh::LoadMesh(UID mesh_id)
{
    MeshImporter mesh_importer;
    meshes.push_back(static_cast<ResourceMesh*>(mesh_importer.Load(mesh_id)));
}

void Hachiko::ComponentMesh::DrawGui()
{
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (meshes.empty())
        {
            DisplayNotLoadedUI();
            return;
        }

        for (auto mesh : meshes)
        {
            ImGui::Text("%d Triangles\n%d vertices\n%d indices",
                        mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] / 3,
                        mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] / 3,
                        mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]);
            ImGui::Checkbox("Visible", &visible);
        }
    }
}

void Hachiko::ComponentMesh::DisplayNotLoadedUI()
{
    if (ImGui::Button("Add Mesh"))
    {
        ImGuiFileDialog::Instance()->OpenDialog("Select Mesh", "Select Mesh", ".*", "./library/models/", 1, nullptr,
            ImGuiFileDialogFlags_DontShowHiddenFiles | 
            ImGuiFileDialogFlags_HideColumnType | 
            ImGuiFileDialogFlags_DisableCreateDirectoryButton |
            ImGuiFileDialogFlags_HideColumnDate);
    }

    if (ImGuiFileDialog::Instance()->Display("Select Mesh"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            // TODO: If filepath is not asset. Import mesh
            LoadMesh(ImGuiFileDialog::Instance()->GetFilePathName().c_str());
        }

        ImGuiFileDialog::Instance()->Close();
    }
}