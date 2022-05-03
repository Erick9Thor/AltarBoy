#include "core/hepch.h"
#include "ResourceNavMesh.h"

#include "Recast.h"

#include "RecastAlloc.h"
#include "RecastAssert.h"

#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"

#include "SampleInterfaces.h"

#include "DebugUtils/DebugDraw.h"
#include "DebugUtils/RecastDebugDraw.h"
#include "DebugUtils/DetourDebugDraw.h"

#include "modules/ModuleCamera.h"
#include "components/ComponentCamera.h"


Hachiko::ResourceNavMesh::ResourceNavMesh(UID uid) : Resource(uid, Type::NAVMESH)
{
    build_context = new BuildContext();
}


Hachiko::ResourceNavMesh::~ResourceNavMesh()
{
    RELEASE(build_context);
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
    AABB scene_bounds;
    scene->GetNavmeshData(scene_vertices, scene_triangles, scene_normals, scene_bounds);

    // Step 1. Initialize build config.
    rcConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.cs = cell_size;
    cfg.ch = cell_height;
    cfg.walkableSlopeAngle = agent_max_slope;
    cfg.walkableHeight = static_cast<int>(ceilf(agent_height / cfg.ch));
    cfg.walkableClimb = static_cast<int>(floorf(agent_max_climb / cfg.ch));
    cfg.walkableRadius = static_cast<int>(ceilf(agent_radius / cfg.cs));
    cfg.maxEdgeLen = static_cast<int>(edge_max_length / cell_size);
    cfg.maxSimplificationError = edge_max_error;
    cfg.minRegionArea = static_cast<int>(rcSqr(region_min_size)); // Note: area = size*size
    cfg.mergeRegionArea = static_cast<int>(rcSqr(region_merge_size)); // Note: area = size*size
    cfg.maxVertsPerPoly = static_cast<int>(max_vertices_per_poly);
    cfg.detailSampleDist = detail_sample_distance < 0.9f ? 0 : cell_size * detail_sample_distance;
    cfg.detailSampleMaxError = cell_height * detail_sample_max_error;
    // Seen on other engine, we dont use tiling atm
    cfg.tileSize = 1000;
    cfg.borderSize = cfg.walkableRadius + 3; // Reserve enough padding.
    cfg.width = cfg.tileSize + cfg.borderSize * 2;
    cfg.height = cfg.tileSize + cfg.borderSize * 2;

    // Set the navigation were navigation will be built
    rcVcopy(cfg.bmin, scene_bounds.minPoint.ptr());
    rcVcopy(cfg.bmax, scene_bounds.maxPoint.ptr());

    // Step 2. Rasterize input polygon soup.

    // Allocate voxel heightfield where we rasterize our input data to.
    rcHeightfield* solid;
    solid = rcAllocHeightfield();
    if (!solid)
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
        return false;
    }
    if (!rcCreateHeightfield(build_context, *solid, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch))
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
        return false;
    }

    // Allocate array that can hold triangle area types.
    // If you have multiple meshes you need to process, allocate
    // and array which can hold the max number of triangles you need to process.
    int n_triangles = scene_triangles.size() / 3;
    unsigned char* triangle_areas = new unsigned char[n_triangles];
    if (!triangle_areas)
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", n_triangles);
        return false;
    }

    // Find triangles which are walkable based on their slope and rasterize them.
    // If your input data is multiple meshes, you can transform them here, calculate
    // the are type for each of the meshes and rasterize them.
    memset(triangle_areas, 0, n_triangles * sizeof(unsigned char));
    int n_vertices = scene_vertices.size() / 3;

    
    rcMarkWalkableTriangles(build_context, cfg.walkableSlopeAngle, scene_vertices.data(), n_vertices, scene_triangles.data(), n_triangles, triangle_areas);
    if (!rcRasterizeTriangles(build_context, scene_vertices.data(), n_vertices, scene_triangles.data(), triangle_areas, n_triangles, *solid, cfg.walkableClimb))
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Could not rasterize triangles.");
        return false;
    }

    RELEASE(triangle_areas);

    // Step 3. Filter walkables surfaces.

    // Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
    rcFilterLowHangingWalkableObstacles(build_context, cfg.walkableClimb, *solid);
    rcFilterLedgeSpans(build_context, cfg.walkableHeight, cfg.walkableClimb, *solid);
    rcFilterWalkableLowHeightSpans(build_context, cfg.walkableHeight, *solid);

    // Step 4. Partition walkable surface to simple regions.

	// Compact the heightfield so that it is faster to handle from now on.
    // This will result more cache coherent data as well as the neighbours
    // between walkable cells will be calculated.

    rcCompactHeightfield* compact_heightfield = rcAllocCompactHeightfield();
    if (!compact_heightfield)
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
        return false;
    }
    if (!rcBuildCompactHeightfield(build_context, compact_heightfield->walkableHeight, compact_heightfield->walkableClimb, *solid, *compact_heightfield))
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
        return false;
    }

    // Do not keep intermediate resources
    rcFreeHeightField(solid);
    solid = nullptr;

    // Erode the walkable area by agent radius.
    if (!rcErodeWalkableArea(build_context, cfg.walkableRadius, *compact_heightfield))
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
        return false;
    }

    // (Optional) Mark areas.
    //InputGeom* geom;
    //const ConvexVolume* vols = m_geom->getConvexVolumes();
    //for (int i = 0; i < m_geom->getConvexVolumeCount(); ++i)
    //    rcMarkConvexPolyArea(build_context, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *chf);

    // Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
    // There are 3 martitioning methods, each with some pros and cons:
    // 1) Watershed partitioning
    //   - the classic Recast partitioning
    //   - creates the nicest tessellation
    //   - usually slowest
    //   - partitions the heightfield into nice regions without holes or overlaps
    //   - the are some corner cases where this method creates produces holes and overlaps
    //      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
    //      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
    //   * generally the best choice if you precompute the nacmesh, use this if you have large open areas
    // 2) Monotone partioning
    //   - fastest
    //   - partitions the heightfield into regions without holes and overlaps (guaranteed)
    //   - creates long thin polygons, which sometimes causes paths with detours
    //   * use this if you want fast navmesh generation
    // 3) Layer partitoining
    //   - quite fast
    //   - partitions the heighfield into non-overlapping regions
    //   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
    //   - produces better triangles than monotone partitioning
    //   - does not have the corner cases of watershed partitioning
    //   - can be slow and create a bit ugly tessellation (still better than monotone)
    //     if you have large open areas with small obstacles (not a problem if you use tiles)
    //   * good choice to use for tiled navmesh with medium and small sized tiles

    if (partition_type == SAMPLE_PARTITION_WATERSHED)
    {
        // Prepare for region partitioning, by calculating distance field along the walkable surface.
        if (!rcBuildDistanceField(build_context, *compact_heightfield))
        {
            build_context->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
            return false;
        }

        // Partition the walkable surface into simple regions without holes.
        if (!rcBuildRegions(build_context, *compact_heightfield, 0, cfg.minRegionArea, cfg.mergeRegionArea))
        {
            build_context->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
            return false;
        }
    }
    else if (partition_type == SAMPLE_PARTITION_MONOTONE)
    {
        // Partition the walkable surface into simple regions without holes.
        // Monotone partitioning does not need distancefield.
        if (!rcBuildRegionsMonotone(build_context, *compact_heightfield, 0, cfg.minRegionArea, cfg.mergeRegionArea))
        {
            build_context->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
            return false;
        }
    }
    else // SAMPLE_PARTITION_LAYERS
    {
        // Partition the walkable surface into simple regions without holes.
        if (!rcBuildLayerRegions(build_context, *compact_heightfield, 0, cfg.minRegionArea))
        {
            build_context->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
            return false;
        }
    }

    // Step 5. Trace and simplify region contours.
    
    rcContourSet* contour_set = rcAllocContourSet();
    contour_set = rcAllocContourSet();
    if (!contour_set)
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
        return false;
    }
    if (!rcBuildContours(build_context, *compact_heightfield, cfg.maxSimplificationError, cfg.maxEdgeLen, *contour_set))
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
        return false;
    }

    // Step 6. Build polygons mesh from contours.

    rcPolyMesh* poly_mesh = rcAllocPolyMesh();
    if (!poly_mesh)
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
        return false;
    }
    if (!rcBuildPolyMesh(build_context, *contour_set, cfg.maxVertsPerPoly, *poly_mesh))
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
        return false;
    }

    // Step 7. Create detail mesh which allows to access approximate height on each polygon.

    rcPolyMeshDetail* detail_mesh = rcAllocPolyMeshDetail();
    if (!detail_mesh)
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
        return false;
    }

    if (!rcBuildPolyMeshDetail(build_context, *poly_mesh, *compact_heightfield, cfg.detailSampleDist, cfg.detailSampleMaxError, *detail_mesh))
    {
        build_context->log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
        return false;
    }

    rcFreeCompactHeightfield(compact_heightfield);
    compact_heightfield = nullptr;
    rcFreeContourSet(contour_set);
    contour_set = nullptr;

    // At this point the navigation mesh data is ready
    // (Optional) Step 8. Create Detour data from Recast poly mesh.
    
    // Only build the detour navmesh if we do not exceed the limit.
    if (cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
    {
        unsigned char* navigation_data = 0;
        int navigation_data_size = 0;

        // Update poly flags from areas.
        for (int i = 0; i < poly_mesh->npolys; ++i)
        {
            if (poly_mesh->areas[i] == RC_WALKABLE_AREA)
            {
                poly_mesh->areas[i] = SAMPLE_POLYAREA_GROUND;
                poly_mesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
            }
        }

        dtNavMeshCreateParams params;
        memset(&params, 0, sizeof(params));
        params.verts = poly_mesh->verts;
        params.vertCount = poly_mesh->nverts;
        params.polys = poly_mesh->polys;
        params.polyAreas = poly_mesh->areas;
        params.polyFlags = poly_mesh->flags;
        params.polyCount = poly_mesh->npolys;
        params.nvp = poly_mesh->nvp;
        params.detailMeshes = detail_mesh->meshes;
        params.detailVerts = detail_mesh->verts;
        params.detailVertsCount = detail_mesh->nverts;
        params.detailTris = detail_mesh->tris;
        params.detailTriCount = detail_mesh->ntris;
        // TODO (optional): Pass Off mesh connections if needed (we dont build m geom so we would have to see how to get them)
        // params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
        // params.offMeshConRad = m_geom->getOffMeshConnectionRads();
        // params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
        // params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
        // params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
        // params.offMeshConUserID = m_geom->getOffMeshConnectionId();
        // params.offMeshConCount = m_geom->getOffMeshConnectionCount();
        params.walkableHeight = agent_height;
        params.walkableRadius = agent_radius;
        params.walkableClimb = agent_max_climb;
        rcVcopy(params.bmin, poly_mesh->bmin);
        rcVcopy(params.bmax, poly_mesh->bmax);
        params.cs = cfg.cs;
        params.ch = cfg.ch;
        params.buildBvTree = true;

        if (!dtCreateNavMeshData(&params, &navigation_data, &navigation_data_size))
        {
            build_context->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
            return false;
        }

        navmesh = dtAllocNavMesh();
        if (!navmesh)
        {
            dtFree(navigation_data);
            build_context->log(RC_LOG_ERROR, "Could not create Detour navmesh");
            return false;
        }

        dtStatus status;

        status = navmesh->init(navigation_data, navigation_data_size, DT_TILE_FREE_DATA);
        if (dtStatusFailed(status))
        {
            dtFree(navigation_data);
            build_context->log(RC_LOG_ERROR, "Could not init Detour navmesh");
            return false;
        }

        navigation_query = dtAllocNavMeshQuery();
        status = navigation_query->init(navmesh, 2048);
        if (dtStatusFailed(status))
        {
            build_context->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
            return false;
        }
    }

    build_context->log(RC_LOG_PROGRESS, ">> Polymesh: %d vertices  %d polygons", poly_mesh->nverts, poly_mesh->npolys);
    // Info on: https://github.com/recastnavigation/recastnavigation/blob/master/RecastDemo/Source/Sample_SoloMesh.cpp

    return true;
}

void Hachiko::ResourceNavMesh::DebugDraw()
{
    if (navmesh)
    {
        unsigned char flags = DU_DRAWNAVMESH_OFFMESHCONS | DU_DRAWNAVMESH_CLOSEDLIST;
        DebugDrawGL m_dd;
        ComponentCamera* camera = App->camera->GetMainCamera();

        glUseProgram(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        constexpr bool transpose = true;
        glLoadMatrixf(camera->GetProjectionMatrix(transpose).ptr());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixf(camera->GetViewMatrix(transpose).ptr());

        glDepthMask(GL_FALSE);
        duDebugDrawNavMeshWithClosedList(&m_dd, *navmesh, *navigation_query, flags);
        glDepthMask(GL_TRUE);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    
}

void Hachiko::ResourceNavMesh::CleanUp()
{
	dtFreeNavMesh(navmesh);
	navmesh = nullptr;
    dtFreeNavMeshQuery(navigation_query);
    navigation_query = nullptr;
}

