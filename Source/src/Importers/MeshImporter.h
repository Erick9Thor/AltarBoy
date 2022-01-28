#pragma once

class ResourceMesh;
struct aiMesh;

namespace MeshImporter
{
	ResourceMesh* Import(const aiMesh* ai_mesh);
	void Save(ResourceMesh* r_mesh, char* file_path);
	ResourceMesh* LoadMesh(const char* file_path);
};
