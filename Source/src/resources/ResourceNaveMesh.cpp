#include "core/hepch.h"
#include "ResourceNaveMesh.h"

void Hachiko::ResourceNavMesh::SetupNavMeshParams()
{

}

bool Hachiko::ResourceNavMesh::Build(Scene* scene)
{
    if (!scene)
    {
        HE_LOG("Error: No mesh was specified.");
        return false;
    }

    CleanUp();

    std::vector<float> scene_vertices;
    std::vector<int> scene_triangles;
    std::vector<float> scene_normals;
    scene->GetNavmeshData(scene_vertices, scene_triangles, scene_normals);

    // Step 1. Initialize build config.
    // Step 2. Rasterize input polygon soup.
    // Step 3. Filter walkables surfaces.
    // Step 4. Partition walkable surface to simple regions.
    // Step 5. Trace and simplify region contours.
    // Step 6. Build polygons mesh from contours.
    // Step 7. Create detail mesh which allows to access approximate height on each polygon.
    // (Optional) Step 8. Create Detour data from Recast poly mesh.
    // Info on: https://github.com/recastnavigation/recastnavigation/blob/master/RecastDemo/Source/Sample_SoloMesh.cpp

    return true;
}

void Hachiko::ResourceNavMesh::DebugDraw() {
}

void Hachiko::ResourceNavMesh::CleanUp()
{
	dtFreeNavMesh(navMesh);
	navMesh = nullptr;
}

