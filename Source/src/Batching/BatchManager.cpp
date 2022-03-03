#include "core/hepch.h"
#include "BatchManager.h"

#include "Application.h"
#include "../Core/GameObject.h"
#include "../Components/ComponentTransform.h"
#include "../Components/ComponentMesh.h"
#include "../Resources/ResourceMesh.h"
#include "../Modules/ModuleProgram.h"

Hachiko::BatchManager::~BatchManager() {
    CleanUp();
}

void Hachiko::BatchManager::AddMesh(const ComponentMesh* mesh)
{
    const ResourceMesh* resource = mesh->GetResource();

    // Find matching batch to include the mesh
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        if (geometry_batch->batch->layout == resource->layout)
        {
            geometry_batch->AddMesh(mesh);
            return;
        }            
    }

    // Create new batch if there are no matching ones
    GeometryBatch* new_batch = new GeometryBatch(resource->layout);
    new_batch->AddMesh(mesh);
    geometry_batches.push_back(new_batch);
}

void Hachiko::BatchManager::BuildBatches()
{
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        geometry_batch->GenerateBatch();
    }
}

void Hachiko::BatchManager::DrawBatches()
{
    static float4x4 identity = float4x4::identity;
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        // Binds meshes and transforms
        geometry_batch->Bind();
        App->program->GetMainProgram()->BindUniformFloat4x4("model", identity.ptr());
        // bind materials array
        auto& commands = geometry_batch->GetCommands();
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, commands.size(), 0);
    }
}

void Hachiko::BatchManager::CleanUp()
{
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        delete geometry_batch;
    }
    geometry_batches.clear();

    for (TextureBatch* texture_batch : texture_batches)
    {
        delete texture_batch;
    }
    texture_batches.clear();
}
