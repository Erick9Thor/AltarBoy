#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "../Application.h"
#include "../Modules/ModuleProgram.h"
#include "../Modules/ModuleRender.h"
#include "../Modules/ModuleCamera.h"
#include "../Program.h"

#include "glew.h"
#include <imgui.h>

ComponentMesh::ComponentMesh(GameObject* conatiner)
	: Component(Component::Type::Mesh, conatiner)
{
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Load(const aiMesh* mesh)
{
	Import(mesh);
	GenerateBuffers();
	GenerateAABB();
	loaded = true;
}

// TODO: Move to importer
void ComponentMesh::Import(const aiMesh* mesh)
{
	buffer_sizes[Buffers::b_vertices] = mesh->mNumVertices * 3;
	vertices = new float[buffer_sizes[Buffers::b_vertices]];
	memcpy(vertices, mesh->mVertices, buffer_sizes[Buffers::b_vertices] * sizeof(float));

	// TODO: use mesh->HasFaces(), mesh->HasNormals() and mesh->HasTextureCoords() to load if exists
	buffer_sizes[Buffers::b_normals] = mesh->mNumVertices * 3;
	normals = new float[buffer_sizes[Buffers::b_normals]];
	memcpy(normals, mesh->mNormals, buffer_sizes[Buffers::b_normals] * sizeof(float));

	buffer_sizes[Buffers::b_tex_coords] = mesh->mNumVertices * 2;
	tex_coords = new float[buffer_sizes[Buffers::b_tex_coords]];
	for (unsigned i = 0; i < mesh->mNumVertices; i++)
		memcpy(&tex_coords[i * 2], &mesh->mTextureCoords[0][i], 2 * sizeof(unsigned));

	buffer_sizes[Buffers::b_indices] = mesh->mNumFaces * 3;
	indices = new unsigned[buffer_sizes[Buffers::b_indices]];
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
		memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
}

void ComponentMesh::GenerateBuffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Positions (3 values per coord)
	glGenBuffers(1, &buffer_ids[Buffers::b_vertices]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_ids[Buffers::b_vertices]);
	glBufferData(GL_ARRAY_BUFFER, buffer_sizes[Buffers::b_vertices] * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Normals (3 values per coord)
	glGenBuffers(1, &buffer_ids[Buffers::b_normals]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_ids[Buffers::b_normals]);
	glBufferData(GL_ARRAY_BUFFER, buffer_sizes[Buffers::b_normals] * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(1);

	// Texture Coords (2 values per coord)
	glGenBuffers(1, &buffer_ids[Buffers::b_tex_coords]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_ids[Buffers::b_tex_coords]);
	glBufferData(GL_ARRAY_BUFFER, buffer_sizes[Buffers::b_tex_coords] * sizeof(float), tex_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(2);

	// Indices (1 value)
	glGenBuffers(1, &buffer_ids[Buffers::b_indices]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_ids[Buffers::b_indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_sizes[Buffers::b_indices] * sizeof(unsigned), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ComponentMesh::GenerateAABB()
{
	bounding_box.SetFrom((float3*) vertices, buffer_sizes[Buffers::b_vertices] / 3);
}

void ComponentMesh::Draw(ComponentCamera* camera, Program* program)
{
	assert(loaded == true);
	program->BindUniformFloat4x4("model", &game_object->GetComponent<ComponentTransform>()->GetTransform()[0][0]);

	ComponentMaterial* material = game_object->GetComponent<ComponentMaterial>();
	App->program->UpdateMaterial(material);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, buffer_sizes[Buffers::b_indices], GL_UNSIGNED_INT, nullptr);
}

void ComponentMesh::CleanUp()
{
	if (loaded)
	{
		glDeleteBuffers(1, &buffer_ids[Buffers::b_indices]);
		glDeleteBuffers(1, &buffer_ids[Buffers::b_vertices]);
		glDeleteBuffers(1, &buffer_ids[Buffers::b_normals]);
		glDeleteBuffers(1, &buffer_ids[Buffers::b_tex_coords]);

		delete[] indices;
		delete[] vertices;
		delete[] normals;
		delete[] tex_coords;

		buffer_sizes[Buffers::b_indices] = 0;
		buffer_sizes[Buffers::b_vertices] = 0;
		buffer_sizes[Buffers::b_normals] = 0;
		buffer_sizes[Buffers::b_tex_coords] = 0;
	}
	loaded = false;
}

void ComponentMesh::DrawGui()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("%d Triangles\n%d vertices\n%d indices", buffer_sizes[Buffers::b_indices] / 3, 
			buffer_sizes[Buffers::b_vertices] / 3, buffer_sizes[Buffers::b_indices]);
		ImGui::Checkbox("Visible", &visible);
	}
}

void ComponentMesh::Save(JsonFormaterValue j_component) const
{
	// jComponent["MeshId"] = mesh.id
}

void ComponentMesh::Load(JsonFormaterValue j_component)
{
	// aiMesh* mesh = App->scene_manager->GetActiveScene().mMeshes[jComponent["MeshId"]];
	// Load(mesh);
}
