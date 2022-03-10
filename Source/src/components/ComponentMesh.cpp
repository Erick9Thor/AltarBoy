#include "core/hepch.h"

#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "modules/ModuleProgram.h"
#include "importers/MeshImporter.h"

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

//void Hachiko::ComponentMesh::Import(const aiMesh* mesh)
//{
//    // TODO: This is ugly. Maybe we should find a better approach,
//    // for example MeshImporter::Import can take resource as ref
//    // and do this check internally.
//
//    if (resource != nullptr && resource->loaded)
//    {
//        resource->CleanUp();
//    }
//
//    resource = MeshImporter::Import(mesh);
//}

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

void Hachiko::ComponentMesh::LoadModel(const char* model_path)
{

}

//void Hachiko::ComponentMesh::Save(JsonFormatterValue j_component) const
//{
//    MeshImporter::Save(resource, game_object->getUID());
//}
//
//void Hachiko::ComponentMesh::Load(JsonFormatterValue j_component)
//{
//    // TODO: This is ugly. Maybe we should find a better approach,
//    // for example MeshImporter::Import can take resource as ref
//    // and do this check internally.
//
//    if (resource != nullptr && resource->loaded)
//    {
//        resource->CleanUp();
//    }
//
//    resource = MeshImporter::Load(game_object->getUID());
//}

void Hachiko::ComponentMesh::DrawGui()
{
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
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
