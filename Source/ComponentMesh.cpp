#include "ComponentMesh.h"

#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"

#include "glew.h"

ComponentMesh::ComponentMesh(GameObject* conatiner) : Component(Component::Type::Mesh, conatiner)
{
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Load(const aiMesh* mesh)
{
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
	GenerateAABB(mesh);
	loaded = true;
}

void ComponentMesh::LoadVBO(const aiMesh* mesh)
{

	num_vertices = mesh->mNumVertices;
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * num_vertices;

	std::vector<Vertex> vertexs;
	vertexs.reserve(num_vertices);

	for (unsigned i = 0; i < num_vertices; ++i)
	{
		Vertex v;
		v.position = float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		v.normal = float3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		v.tex_coords = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertexs.push_back(v);
	}

	// Generate, activate and fill buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertices, &vertexs[0], GL_STATIC_DRAW);
	vertexs.clear();
}

void ComponentMesh::LoadEBO(const aiMesh* mesh)
{
	num_indices = mesh->mNumFaces * 3;

	// Generate & activate buffer
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Compute size, each face is 3 vertex indexes
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;

	// Update buffer data attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);

	// Map buffer and fill it in a custom manner
	unsigned* indices = (unsigned*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3);
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void ComponentMesh::CreateVAO()
{
	static const unsigned stride = sizeof(Vertex);
	// Generate vao, activate & bind buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Define layout, index, size, type, normalized, stride, pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); // Positions
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (3))); // Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (3 + 3))); // Texture coords
}

void ComponentMesh::GenerateAABB(const aiMesh* mesh)
{
	bounding_box.SetFrom((float3*)&mesh->mVertices[0], mesh->mNumVertices);
}

void ComponentMesh::Draw()
{
	assert(loaded == true);
	// TODO: Get material and transform components to draw sadge
	App->program->Activate();
	
	App->program->BindUniformFloat4x4("model", &game_object->GetComponent<ComponentTransform>()->GetTransform()[0][0]);
	App->program->BindUniformFloat4x4("view", &App->camera->getMainCamera()->GetViewMatrix()[0][0]);
	App->program->BindUniformFloat4x4("proj", &App->camera->getMainCamera()->GetProjectionMatrix()[0][0]);

	//App->texture->Bind(model_textures[texture_index].id);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
	App->texture->Unbind();

	App->program->Deactivate();
}

void ComponentMesh::CleanUp()
{
	assert(loaded == true);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	loaded = false;
}