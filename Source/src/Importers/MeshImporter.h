#pragma once

#include "../Utils/UID.h"

class ResourceMesh;
struct aiMesh;

namespace MeshImporter
{
	ResourceMesh* Import(const aiMesh* ai_mesh);
	void Save(ResourceMesh* r_mesh, UID uid);
	ResourceMesh* Load(UID uid);
};
