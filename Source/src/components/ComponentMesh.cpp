#include "core/hepch.h"

#include "modules/ModuleProgram.h"
#include "modules/ModuleResources.h"

#include "resources/ResourceMesh.h"

#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

Hachiko::ComponentMesh::ComponentMesh(GameObject* container, UID id, ResourceMesh* res) 
    : Component(Type::MESH, container)
{
    if (res != nullptr)
    {
        mesh = res;
    }
}

void Hachiko::ComponentMesh::Draw(ComponentCamera* camera, Program* program)
{
    if (mesh == nullptr)
    {
        return;
    }
    
    program->BindUniformFloat4x4("model", game_object->GetTransform()->GetGlobalMatrix().ptr());

    const ComponentMaterial* material = game_object->GetComponent<ComponentMaterial>();
    App->program->UpdateMaterial(material);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)], GL_UNSIGNED_INT, nullptr);
}

void Hachiko::ComponentMesh::DrawStencil(ComponentCamera* camera, Program* program) const
{
    if (mesh == nullptr)
    {
        return;
    }
    
    program->BindUniformFloat4x4("model", game_object->GetTransform()->GetGlobalMatrix().ptr());

    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)], GL_UNSIGNED_INT, nullptr);
}

void Hachiko::ComponentMesh::LoadMesh(UID mesh_id)
{
    mesh = static_cast<ResourceMesh*> (App->resources->GetResource(Resource::Type::MESH, mesh_id));
}

void Hachiko::ComponentMesh::DrawGui()
{
    ImGui::PushID(this);
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (mesh == nullptr)
        {
            return;
        }

        ImGui::Text("%d Triangles\n%d vertices\n%d indices",
                    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] / 3,
                    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)],
                    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]);
        ImGui::Checkbox("Visible", &visible);
    }
    ImGui::PopID();
}

void Hachiko::ComponentMesh::Save(YAML::Node& node) const
{
    node[MODEL_FILE_PATH] = asset_path;
    node[MODEL_NAME] = model_name;
    node[NODE_MESH_INDEX] = mesh_index;
}

void Hachiko::ComponentMesh::Load(const YAML::Node& node)
{
    model_name = node[MODEL_NAME].as<std::string>();
    mesh_index = node[NODE_MESH_INDEX].as<int>();
    SetID(node[COMPONENT_ID].as<UID>());
    LoadMesh(node[COMPONENT_ID].as<UID>());
}
