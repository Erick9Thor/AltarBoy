#pragma once

class ResourceMesh;
struct aiMesh;

class MeshImporter
{
	ResourceMesh* Import(const aiMesh* ai_mesh);
	void Save(ResourceMesh* r_mesh, char* file_path);
	ResourceMesh* LoadMesh(char* file_path);
};
