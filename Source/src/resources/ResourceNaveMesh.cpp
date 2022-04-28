#include "core/hepch.h"
#include "ResourceNaveMesh.h"

void Hachiko::ResourceNavMesh::SetupNavMeshParams()
{

}

bool Hachiko::ResourceNavMesh::Build(const ComponentMesh* terrainMesh)
{
	if (!terrainMesh)
	{
		HE_LOG("Error: No mesh was specified.");
		return false;
	}

	CleanUp();

	const float* vertices = terrainMesh->GetVertices();
	const int nverts = terrainMesh->GetBufferSize(Hachiko::ResourceMesh::Buffers::VERTICES);
	const unsigned* triangles = terrainMesh->GetIndices();
	const int ntris = terrainMesh->GetBufferSize(Hachiko::ResourceMesh::Buffers::INDICES);
	const float* normals = terrainMesh->GetNormals();
	const int nnorms = terrainMesh->GetBufferSize(Hachiko::ResourceMesh::Buffers::NORMALS);

	

	return true;
}

void Hachiko::ResourceNavMesh::DebugDraw()
{
}

void Hachiko::ResourceNavMesh::CleanUp()
{
	dtFreeNavMesh(navMesh);
	navMesh = nullptr;
}

