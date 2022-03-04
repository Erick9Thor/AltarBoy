#include "core/hepch.h"

#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "modules/ModuleProgram.h"
#include "importers/MeshImporter.h"

#include "core/Scene.h"

Hachiko::ComponentMesh::ComponentMesh(GameObject* container) :
    Component(Type::MESH, container)
{
    if (container->scene_owner)
    {
        container->scene_owner->OnMeshesChanged();
    }
}

Hachiko::ComponentMesh::~ComponentMesh()
{
    if (game_object->scene_owner)
    {
        game_object->scene_owner->OnMeshesChanged();
    }
    RELEASE(resource);
}

void Hachiko::ComponentMesh::Import(const aiMesh* mesh)
{
    // TODO: This is ugly. Maybe we should find a better approach,
    // for example MeshImporter::Import can take resource as ref
    // and do this check internally.

    if (resource != nullptr && resource->loaded)
    {
        resource->CleanUp();
    }

    resource = MeshImporter::Import(mesh);
}

void Hachiko::ComponentMesh::Draw(ComponentCamera* camera, Program* program)
{
    assert(resource->loaded == true);
    program->BindUniformFloat4x4("model", game_object->GetTransform()->GetMatrix().ptr());

    const ComponentMaterial* material = game_object->GetComponent<ComponentMaterial>();
    App->program->UpdateMaterial(material);

    glBindVertexArray(resource->vao);
    glDrawElements(GL_TRIANGLES, resource->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)], GL_UNSIGNED_INT, nullptr);
}

void Hachiko::ComponentMesh::DrawStencil(ComponentCamera* camera, Program* program) const
{
    assert(resource->loaded == true);
    program->BindUniformFloat4x4("model", game_object->GetTransform()->GetMatrix().ptr());

    glBindVertexArray(resource->vao);
    glDrawElements(GL_TRIANGLES, resource->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)], GL_UNSIGNED_INT, nullptr);
}

void Hachiko::ComponentMesh::Save(JsonFormatterValue j_component) const
{
    MeshImporter::Save(resource, game_object->getUID());
}

void Hachiko::ComponentMesh::Load(JsonFormatterValue j_component)
{
    // TODO: This is ugly. Maybe we should find a better approach,
    // for example MeshImporter::Import can take resource as ref
    // and do this check internally.

    if (resource != nullptr && resource->loaded)
    {
        resource->CleanUp();
    }

    resource = MeshImporter::Load(game_object->getUID());
}

void Hachiko::ComponentMesh::DrawGui()
{
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("%d Triangles\n%d vertices\n%d indices",
                    resource->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] / 3,
                    resource->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] / 3,
                    resource->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]);
        ImGui::Checkbox("Visible", &visible);
    }
}
