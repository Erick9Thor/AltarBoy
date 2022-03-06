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
    assert(!meshes.empty());
    // TODO: Why we take care of other components?
    program->BindUniformFloat4x4("model", &game_object->GetComponent<ComponentTransform>()->GetTransform()[0][0]);

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
    assert(!meshes.empty());
    program->BindUniformFloat4x4("model", &game_object->GetComponent<ComponentTransform>()->GetTransform()[0][0]);

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
    // TODO: Only drawing for the first mesh
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("%d Triangles\n%d vertices\n%d indices",
                    meshes[0]->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] / 3,
                    meshes[0]->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] / 3,
                    meshes[0]->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]);
        ImGui::Checkbox("Visible", &visible);
    }
}
