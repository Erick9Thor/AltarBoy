#pragma once

#include "utils/UUID.h"
#include "assimp/scene.h"

namespace Hachiko
{
    class ResourceMesh;

    namespace MeshImporter
    {
        ResourceMesh* Import(const aiMesh* ai_mesh);
        ResourceMesh* Import(const aiMesh* ai_mesh, const UID id);
        void Save(ResourceMesh* r_mesh, UID uid);
        ResourceMesh* Load(UID uid);
    }; // namespace MeshImporter
}
