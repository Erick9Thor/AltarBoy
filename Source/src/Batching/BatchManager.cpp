#include "core/hepch.h"
#include "BatchManager.h"

#include "Application.h"
#include "../Core/GameObject.h"
#include "../Components/ComponentTransform.h"
#include "../Components/ComponentMesh.h"
#include "../Resources/ResourceMesh.h"
#include "../Modules/ModuleProgram.h"

Hachiko::BatchManager::BatchManager() 
{
    GenerateDynamicBuffers();
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
    auto* mesh = game_object->GetComponent<ComponentMesh>();
    if (mesh == nullptr)
    {
        return;
    }
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

void Hachiko::BatchManager::AddDrawComponent(const ComponentMesh* mesh)
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

void Hachiko::BatchManager::GenerateDynamicBuffers() 
{
    glGenBuffers(1, &material_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, material_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, material_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
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
        geometry_batch->Draw();
        App->program->GetMainProgram()->BindUniformFloat4x4("model", identity.ptr());
        // Bind texture batch
        geometry_batch->textureBatch->UpdateTextureBatch();
        geometry_batch->textureBatch->UpdateMaterials(material_ssbo);
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
