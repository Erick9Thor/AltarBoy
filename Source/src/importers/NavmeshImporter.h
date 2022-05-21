#pragma once
#pragma once
#include <cassert>
#include "Importer.h"
#include "DetourTileCache.h"
#include "DetourNavMesh.h"

namespace Hachiko
{
    class ResourceNavmesh;
    class Scene;

    class NavmeshImporter final : public Importer
    {
    public:

        friend class SceneImporter;

        struct TileCacheSetHeader
        {
            int magic;
            int version;
            int numTiles;
            dtNavMeshParams meshParams;
            dtTileCacheParams cacheParams;
        };

        struct TileCacheTileHeader
        {
            dtCompressedTileRef tileRef;
            int dataSize;
        };

        NavmeshImporter() = default;
        ~NavmeshImporter() override = default;

        void Import(const char* path, YAML::Node& meta) override
        {
            // Scene is in charge of generating navmesh resources
            assert(false && "This should not be called since we dont have navmesh assets");
        }
        void Save(UID id, const Resource* resource) override;
        Resource* Load(UID id) override;

    private:
        void CreateNavmeshFromScene(Scene* scene, UID id);
    };
} // namespace Hachiko
