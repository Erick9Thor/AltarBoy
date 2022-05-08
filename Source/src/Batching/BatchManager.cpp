#include "core/hepch.h"
#include "BatchManager.h"

#include "Application.h"
#include "core/GameObject.h"
#include "components/ComponentTransform.h"
#include "components/ComponentMeshRenderer.h"
#include "resources/ResourceMesh.h"
#include "modules/ModuleProgram.h"

Hachiko::BatchManager::BatchManager() 
{
}

Hachiko::BatchManager::~BatchManager()
{
    CleanUp();
}

void Hachiko::BatchManager::CollectMeshes(const GameObject* game_object)
{
    CollectMesh(game_object);

    for (GameObject* child : game_object->children)
    {
        CollectMeshes(child);
    }    
}

void Hachiko::BatchManager::CollectMesh(const GameObject* game_object)
{
    const std::vector<Component*> components = game_object->GetComponents();
    for (unsigned i = 0; i < components.size(); ++i)
    {
        if (components[i]->GetType() == Component::Type::MESH_RENDERER)
        {
            const ComponentMeshRenderer* mesh_renderer = static_cast<ComponentMeshRenderer*>(components[i]);
            const ResourceMesh* resource = mesh_renderer->GetResource();

            // Find matching batch to include the mesh
            for (GeometryBatch* geometry_batch : geometry_batches)
            {
                if (geometry_batch->batch->layout == resource->layout)
                {
                    geometry_batch->AddMesh(mesh_renderer);
                    return;
                }
            }

            // Create new batch if there are no matching ones
            GeometryBatch* new_batch = new GeometryBatch(resource->layout);
            new_batch->AddMesh(mesh_renderer);
            geometry_batches.push_back(new_batch);
        }
    }
}

void Hachiko::BatchManager::AddDrawComponent(const ComponentMeshRenderer* mesh)
{
    const ResourceMesh* resource = mesh->GetResource();

    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        if (geometry_batch->batch->layout == resource->layout)
        {
            // We assume that the resource is already contained in the batch
            geometry_batch->AddDrawComponent(mesh);
            return;
        }
    }
}

void Hachiko::BatchManager::BuildBatches()
{
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        geometry_batch->BuildBatch();
    }
}

void Hachiko::BatchManager::DrawBatches()
{
    static float4x4 identity = float4x4::identity;
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        geometry_batch->ImGuiWindow(); // DEBUG

        // Binds meshes and transforms
        geometry_batch->UpdateWithTextureBatch();
        //App->program->GetMainProgram()->BindUniformFloat4x4("model", identity.ptr());
        // Bind texture batch
        // bind materials array
        auto& commands = geometry_batch->GetCommands();
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, commands.size(), 0);
    }
}

void Hachiko::BatchManager::ClearBatchesDrawList()
{
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        geometry_batch->ClearDrawList();
    }
}

void Hachiko::BatchManager::CleanUp()
{
    for (GeometryBatch* geometry_batch : geometry_batches)
    {
        delete geometry_batch;
    }
    geometry_batches.clear();

    //glDeleteBuffers(1, &material_ssbo);
}
