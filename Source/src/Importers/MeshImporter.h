#pragma once

#include "Utils/UUID.h"
#include "assimp/scene.h"

namespace Hachiko
{
    class ResourceMesh;

    namespace MeshImporter
    {
        ResourceMesh* Import(const aiMesh* ai_mesh);
        void Save(ResourceMesh* r_mesh, UID uid);
        ResourceMesh* Load(UID uid);
    }; // namespace MeshImporter
}
