#include "MeshImporter.h"
#include "../Utils/UID.h"

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
	
	// Save resource

	return r_mesh;
}

void MeshImporter::Save(ResourceMesh* r_mesh, char* file_path)
{

}

ResourceMesh* MeshImporter::LoadMesh(const char* file_path)
{
	return nullptr;
}
