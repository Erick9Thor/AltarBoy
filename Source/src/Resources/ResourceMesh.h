#pragma once
#include "Resource.h"

#include "MathGeoLib.h"


class ResourceMesh : public Resource
{
public:

	enum Buffers
	{
		b_indices = 0,
		b_vertices,
		b_normals,
		b_tex_coords,
		n_buffers,
	};

	ResourceMesh(UID id);
	void GenerateBuffers();
	void GenerateAABB();

	void CleanUp();

	bool loaded = false;

	AABB bounding_box;

	// Store the ids of buffers in opengl
	unsigned vbo, ebo;
	unsigned vao;
	unsigned buffer_ids[n_buffers];

	// Track buffer sizes
	unsigned buffer_sizes[n_buffers];

	// In memory buffers
	unsigned* indices;
	float* vertices;
	float* normals;
	float* tex_coords;
};
