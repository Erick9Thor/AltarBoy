#include "core/hepch.h"

#include "NavmeshImporter.h"
#include "resources/ResourceNavMesh.h"
#include "modules/ModuleNavigation.h"

void Hachiko::NavmeshImporter::Save(UID id, const Resource* resource)
{
    const ResourceNavMesh* navmesh = static_cast<const ResourceNavMesh*>(resource);
    const std::string file_path = GetResourcePath(Resource::Type::NAVMESH, id);
    
    if (!navmesh->tile_cache)
    {
        // Store empty file which means no navmes or return 0 which also will mean no navmesh
    }

    unsigned file_size = 0;
    dtTileCache* tiles = navmesh->tile_cache;
    TileCacheSetHeader header;

    // Compute total file size
    file_size += sizeof(TileCacheSetHeader);
    for (unsigned i = 0; i < tiles->getTileCount(); ++i)
    {
        const dtCompressedTile* tile = tiles->getTile(i);
        if (!tile || !tile->header || !tile->dataSize)
            continue;

        file_size += sizeof(TileCacheTileHeader);
        file_size += tile->dataSize;
        header.numTiles++;
    }

    memcpy(&header.cacheParams, tiles->getParams(), sizeof(dtTileCacheParams));
    memcpy(&header.meshParams, navmesh->navmesh->getParams(), sizeof(dtNavMeshParams));

    const auto file_buffer = new char[file_size];
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    size_bytes = sizeof(TileCacheSetHeader);
    memcpy(cursor, &header, size_bytes);
    cursor += size_bytes;

    for (unsigned i = 0; i < tiles->getTileCount(); ++i)
    {
        const dtCompressedTile* tile = tiles->getTile(i);
        if (!tile || !tile->header || !tile->dataSize)
            continue;

        TileCacheTileHeader tile_header;
        tile_header.tileRef = tiles->getTileRef(tile);
        tile_header.dataSize = tile->dataSize;

        size_bytes = sizeof(TileCacheTileHeader);
        memcpy(cursor, &tile_header, size_bytes);
        cursor += size_bytes;

        size_bytes = tile_header.dataSize;
        memcpy(cursor, tile->data, size_bytes);
        cursor += size_bytes;
    }

    FileSystem::Save(file_path.c_str(), file_buffer, file_size);
    delete[] file_buffer;
}

Hachiko::Resource* Hachiko::NavmeshImporter::Load(UID id)
{
    const std::string file_path = GetResourcePath(Resource::Type::NAVMESH, id);

    char* file_buffer = FileSystem::Load(file_path.c_str());
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    ResourceNavMesh* navmesh = new ResourceNavMesh(id);
    navmesh->talloc = new LinearAllocator(32000);
    navmesh->tcomp = new FastLZCompressor;
    navmesh->tmproc = new MeshProcess;

    TileCacheSetHeader header;
    size_bytes = sizeof(TileCacheSetHeader);
    memcpy(&header, cursor, size_bytes);
    cursor += size_bytes;

    navmesh->navmesh = dtAllocNavMesh();

    navmesh->navmesh->init(&header.meshParams);

    navmesh->tile_cache = dtAllocTileCache();

    navmesh->tile_cache->init(&header.cacheParams, navmesh->talloc, navmesh->tcomp, navmesh->tmproc);

    for (unsigned i = 0; i < header.numTiles; ++i)
    {
        size_bytes = sizeof(TileCacheTileHeader);
        TileCacheTileHeader tile_header;
        memcpy(&tile_header, cursor, size_bytes);
        cursor += size_bytes;

        if (!tile_header.tileRef || !tile_header.dataSize)
        {
            break;
        }

        unsigned char* data = (unsigned char*)malloc(tile_header.dataSize);
        if (!data)
        {
            break;
        }
        memset(data, 0, tile_header.dataSize);

        size_bytes = tile_header.dataSize;
        memcpy(data, cursor, size_bytes);
        cursor += size_bytes;

        dtCompressedTileRef tile = 0;
        navmesh->tile_cache->addTile(data, tile_header.dataSize, DT_COMPRESSEDTILE_FREE_DATA, &tile);
        if (tile)
        {
            navmesh->tile_cache->buildNavMeshTile(tile, navmesh->navmesh);
        }
    }

    navmesh->navigation_query = dtAllocNavMeshQuery();
    navmesh->navigation_query->init(navmesh->navmesh, 2048);

    navmesh->InitCrowd();
    
    return navmesh;
}

void Hachiko::NavmeshImporter::CreateNavmeshFromScene(Scene* scene, UID id)
{
    ResourceNavMesh* navmesh = App->navigation->BuildNavmesh(scene);
    Save(id, navmesh);
    delete navmesh;
}
