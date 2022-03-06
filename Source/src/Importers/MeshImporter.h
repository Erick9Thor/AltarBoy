#pragma once

class aiMesh;

namespace Hachiko
{
    class ResourceMesh;
    class UID;

    class MeshImporter : public Importer
    {
    public:
        MeshImporter();
        ResourceMesh* Import(const aiMesh* ai_mesh);
        ResourceMesh* Import(const aiMesh* ai_mesh, const UID id);
        void Save(const Resource* mesh) override;
        Resource* Load(const UID uid) override;
    };
}
