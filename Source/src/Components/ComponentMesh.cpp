#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "Application.h"
#include "Modules/ModuleProgram.h"
#include "Modules/ModuleRender.h"
#include "Program.h"

#include "Importers/MeshImporter.h"

#include "glew.h"
#include <imgui.h>

ComponentMesh::ComponentMesh(GameObject* conatiner)
	: Component(Component::Type::MESH, conatiner)
{
}

ComponentMesh::~ComponentMesh()
{
	RELEASE(resource);
}

void ComponentMesh::Import(const aiMesh* mesh)
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

void ComponentMesh::Draw(ComponentCamera* camera, Program* program)
{
	assert(resource->loaded == true);
	program->BindUniformFloat4x4("model", &game_object->GetComponent<ComponentTransform>()->GetTransform()[0][0]);

	ComponentMaterial* material = game_object->GetComponent<ComponentMaterial>();
	App->program->UpdateMaterial(material);

	glBindVertexArray(resource->vao);
	glDrawElements(GL_TRIANGLES, resource->buffer_sizes[ResourceMesh::Buffers::b_indices], GL_UNSIGNED_INT, nullptr);
}

void ComponentMesh::DrawStencil(ComponentCamera* camera, Program* program)
{
	assert(resource->loaded == true);
	program->BindUniformFloat4x4("model", &game_object->GetComponent<ComponentTransform>()->GetTransform()[0][0]);

	glBindVertexArray(resource->vao);
	glDrawElements(GL_TRIANGLES, resource->buffer_sizes[ResourceMesh::Buffers::b_indices], GL_UNSIGNED_INT, nullptr);
}

void ComponentMesh::Save(JsonFormaterValue j_component) const
{
	MeshImporter::Save(resource, game_object->getUID());
}

void ComponentMesh::Load(JsonFormaterValue j_component)
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

void ComponentMesh::DrawGui()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("%d Triangles\n%d vertices\n%d indices", resource->buffer_sizes[ResourceMesh::Buffers::b_indices] / 3, resource->buffer_sizes[ResourceMesh::Buffers::b_vertices] / 3, resource->buffer_sizes[ResourceMesh::Buffers::b_indices]);
		ImGui::Checkbox("Visible", &visible);
	}
}