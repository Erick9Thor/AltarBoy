#pragma once

#include "Utils/UUID.h"

class ResourceMesh;
struct aiMesh;

namespace MeshImporter
{
	ResourceMesh* Import(const aiMesh* ai_mesh);
	void Save(ResourceMesh* r_mesh, Hachiko::UID uid);
	ResourceMesh* Load(Hachiko::UID uid);
};
