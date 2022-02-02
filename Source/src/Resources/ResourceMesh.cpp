#include "ResourceMesh.h"

#include "glew.h"


ResourceMesh::ResourceMesh(UID id)
	: Resource(id, ResourceType::MESH)
{
}

void ResourceMesh::GenerateBuffers()
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

void ResourceMesh::GenerateAABB()
{
	bounding_box.SetFrom((float3*) vertices, buffer_sizes[Buffers::b_vertices] / 3);
}

void ResourceMesh::CleanUp()
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
