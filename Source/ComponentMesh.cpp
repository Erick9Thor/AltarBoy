#include "ComponentMesh.h"

#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "glew.h"

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
	num_vertices = mesh->mNumVertices;
	vertices_buffer_size = mesh->mNumVertices * 3;
	vertices = new float[vertices_buffer_size];
	memcpy(vertices, mesh->mVertices, vertices_buffer_size * sizeof(float));

	// TODO: use mesh->HasFaces(), mesh->HasNormals() and mesh->HasTextureCoords() to load if exists
	normals_buffer_size = mesh->mNumVertices * 3;
	normals = new float[normals_buffer_size];
	memcpy(normals, mesh->mNormals, normals_buffer_size * sizeof(float));

	tex_coords_buffer_size = mesh->mNumVertices * 2;
	tex_coords = new float[tex_coords_buffer_size];
	for (unsigned i = 0; i < mesh->mNumVertices; i++)
		memcpy(&tex_coords[i * 2], &mesh->mTextureCoords[0][i], 2 * sizeof(unsigned));

	num_indices = mesh->mNumFaces * 3;
	indices_buffer_size = num_indices;
	indices = new unsigned[indices_buffer_size];
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
		memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
}

void ComponentMesh::GenerateBuffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Positions (3 values per coord)
	glGenBuffers(1, &vertices_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices_buffer_size * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Normals (3 values per coord)
	glGenBuffers(1, &normals_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals_buffer_size * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(1);

	// Texture Coords (2 values per coord)
	glGenBuffers(1, &tex_coords_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coords_buffer);
	glBufferData(GL_ARRAY_BUFFER, tex_coords_buffer_size * sizeof(float), tex_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(2);

	// Indices (1 value)
	glGenBuffers(1, &indices_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_size * sizeof(unsigned), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ComponentMesh::GenerateAABB()
{
	bounding_box.SetFrom((float3*) &vertices, num_vertices);
}

void ComponentMesh::Draw()
{
	assert(loaded == true);
	// TODO: Get material and transform components to draw sadge
	App->program->Activate();

	App->program->BindUniformFloat4x4("model", &game_object->GetComponent<ComponentTransform>()->GetTransform()[0][0]);
	App->program->BindUniformFloat4x4("view", &App->camera->getMainCamera()->GetViewMatrix()[0][0]);
	App->program->BindUniformFloat4x4("proj", &App->camera->getMainCamera()->GetProjectionMatrix()[0][0]);

	App->texture->Bind(game_object->GetComponent<ComponentMaterial>()->GetTextureId());
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
	App->texture->Unbind();

	App->program->Deactivate();
}

void ComponentMesh::CleanUp()
{
	if (loaded)
	{
		glDeleteBuffers(1, &indices_buffer);
		glDeleteBuffers(1, &vertices_buffer);
		glDeleteBuffers(1, &normals_buffer);
		glDeleteBuffers(1, &tex_coords_buffer);

		delete[] indices;
		delete[] vertices;
		delete[] normals;
		delete[] tex_coords;

		indices_buffer_size = 0;
		vertices_buffer_size = 0;
		normals_buffer_size = 0;
		tex_coords_buffer_size = 0;
	}
	loaded = false;
}

void ComponentMesh::DrawGui()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("%d Triangles\n%d vertices\n%d indices", num_indices / 3, num_vertices, num_indices);
	}
}