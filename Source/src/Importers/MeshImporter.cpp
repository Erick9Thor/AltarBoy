#include "MeshImporter.h"
#include "../Utils/UID.h"

#include "../Application.h"
#include "../Modules/ModuleFileSystem.h"

#include "../Resources/ResourceMesh.h"
#include "assimp/scene.h"

ResourceMesh* MeshImporter::Import(const aiMesh* ai_mesh)
{
	ResourceMesh* r_mesh = new ResourceMesh(GenerateUID());
	r_mesh->buffer_sizes[ResourceMesh::Buffers::b_vertices] = ai_mesh->mNumVertices * 3;
	r_mesh->vertices = new float[r_mesh->buffer_sizes[ResourceMesh::Buffers::b_vertices]];
	memcpy(r_mesh->vertices, ai_mesh->mVertices, r_mesh->buffer_sizes[ResourceMesh::Buffers::b_vertices] * sizeof(float));

	// TODO: use mesh->HasFaces(), mesh->HasNormals() and mesh->HasTextureCoords() to load if exists
	r_mesh->buffer_sizes[ResourceMesh::Buffers::b_normals] = ai_mesh->mNumVertices * 3;
	r_mesh->normals = new float[r_mesh->buffer_sizes[ResourceMesh::Buffers::b_normals]];
	memcpy(r_mesh->normals, ai_mesh->mNormals, r_mesh->buffer_sizes[ResourceMesh::Buffers::b_normals] * sizeof(float));

	r_mesh->buffer_sizes[ResourceMesh::Buffers::b_tex_coords] = ai_mesh->mNumVertices * 2;
	r_mesh->tex_coords = new float[r_mesh->buffer_sizes[ResourceMesh::Buffers::b_tex_coords]];
	for (unsigned i = 0; i < ai_mesh->mNumVertices; i++)
		memcpy(&r_mesh->tex_coords[i * 2], &ai_mesh->mTextureCoords[0][i], 2 * sizeof(unsigned));

	r_mesh->buffer_sizes[ResourceMesh::Buffers::b_indices] = ai_mesh->mNumFaces * 3;
	r_mesh->indices = new unsigned[r_mesh->buffer_sizes[ResourceMesh::Buffers::b_indices]];
	for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i)
		memcpy(&r_mesh->indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));

	r_mesh->GenerateBuffers();
	r_mesh->GenerateAABB();
	r_mesh->loaded = true;
	
	// Save resource

	return r_mesh;
}

void MeshImporter::Save(ResourceMesh* r_mesh, UID uid)
{	
	std::string file_path = std::string(LIBRARY_MESH_FOLDER "/") + std::to_string(uid) + ".mesh";

	unsigned* sizes = r_mesh->buffer_sizes;
	unsigned header[ResourceMesh::Buffers::n_buffers] = {
		sizes[ResourceMesh::Buffers::b_indices],
		sizes[ResourceMesh::Buffers::b_vertices],
		sizes[ResourceMesh::Buffers::b_normals],
		sizes[ResourceMesh::Buffers::b_tex_coords],
	};

	unsigned file_size = 0;
	file_size += sizeof(header);
	file_size += sizeof(unsigned) * sizes[ResourceMesh::Buffers::b_indices];
	file_size += sizeof(float) * sizes[ResourceMesh::Buffers::b_vertices];
	file_size += sizeof(float) * sizes[ResourceMesh::Buffers::b_normals];
	file_size += sizeof(float) * sizes[ResourceMesh::Buffers::b_tex_coords];
	
	char* file_buffer = new char[file_size];
	char* cursor = file_buffer;
	unsigned size_bytes = 0;

	size_bytes = sizeof(header);
	memcpy(cursor, sizes, size_bytes);
	cursor += size_bytes;

	size_bytes = sizeof(unsigned) * sizes[ResourceMesh::Buffers::b_indices];
	memcpy(cursor, r_mesh->indices, size_bytes);
	cursor += size_bytes;

	size_bytes = sizeof(float) * sizes[ResourceMesh::Buffers::b_vertices];
	memcpy(cursor, r_mesh->vertices, size_bytes);
	cursor += size_bytes;
	
	size_bytes = sizeof(float) * sizes[ResourceMesh::Buffers::b_normals];
	memcpy(cursor, r_mesh->normals, size_bytes);
	cursor += size_bytes;

	size_bytes = sizeof(float) * sizes[ResourceMesh::Buffers::b_tex_coords];
	memcpy(cursor, r_mesh->tex_coords, size_bytes);
	cursor += size_bytes;


	App->file_sys->Save(file_path.c_str(), file_buffer, file_size);
	delete[] file_buffer;
}

ResourceMesh* MeshImporter::Load(UID uid)
{
	std::string file_path = std::string(LIBRARY_MESH_FOLDER "/") + std::to_string(uid) + ".mesh";	
	char* file_buffer = App->file_sys->Load(file_path.c_str());
	char* cursor = file_buffer;
	unsigned size_bytes = 0;
		
	// TODO: Manage uid properly
	ResourceMesh* r_mesh = new ResourceMesh(1);

	unsigned header[ResourceMesh::Buffers::n_buffers];
	size_bytes = sizeof(header);
	memcpy(header, cursor, size_bytes);
	cursor += size_bytes;
	
	unsigned* sizes = r_mesh->buffer_sizes;
	sizes[ResourceMesh::Buffers::b_indices] = header[0];
	sizes[ResourceMesh::Buffers::b_vertices] = header[1];
	sizes[ResourceMesh::Buffers::b_normals] = header[2];
	sizes[ResourceMesh::Buffers::b_tex_coords] = header[3];

	size_bytes = sizeof(unsigned) * sizes[ResourceMesh::Buffers::b_indices];
	r_mesh->indices = new unsigned[sizes[ResourceMesh::Buffers::b_indices]];
	memcpy(r_mesh->indices, cursor, size_bytes);
	cursor += size_bytes;

	r_mesh->vertices = new float[sizes[ResourceMesh::Buffers::b_vertices]];
	size_bytes = sizeof(float) * sizes[ResourceMesh::Buffers::b_vertices];
	memcpy(r_mesh->vertices, cursor, size_bytes);
	cursor += size_bytes;

	r_mesh->normals = new float[sizes[ResourceMesh::Buffers::b_normals]];
	size_bytes = sizeof(float) * sizes[ResourceMesh::Buffers::b_normals];
	memcpy(r_mesh->normals, cursor, size_bytes);
	cursor += size_bytes;

	r_mesh->tex_coords = new float[sizes[ResourceMesh::Buffers::b_tex_coords]];
	size_bytes = sizeof(float) * sizes[ResourceMesh::Buffers::b_tex_coords];
	memcpy(r_mesh->tex_coords, cursor, size_bytes);
	cursor += size_bytes;

	r_mesh->GenerateBuffers();
	r_mesh->GenerateAABB();
	r_mesh->loaded = true;

	return r_mesh;
}
