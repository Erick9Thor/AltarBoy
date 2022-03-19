#include "core/hepch.h"
#include "ModuleUserInterface.h"
#include "ModuleSceneManager.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"

#include "core/Scene.h"
#include "core/GameObject.h"
#include "components/ComponentCanvasRenderer.h"
#include "components/ComponentCamera.h"





Hachiko::ModuleUserInterface::ModuleUserInterface() = default;

Hachiko::ModuleUserInterface::~ModuleUserInterface() = default;

bool Hachiko::ModuleUserInterface::Init()
{
    CreateSquare();
    return true;
}

UpdateStatus Hachiko::ModuleUserInterface::Update(float delta)
{
    return UpdateStatus::UPDATE_CONTINUE;
}

bool Hachiko::ModuleUserInterface::CleanUp()
{
    RemoveSquare();
    return true;
}

void Hachiko::ModuleUserInterface::DrawUI(const Scene* scene)
{
    Program* program = App->program->GetUserInterfaceProgram();
    program->Activate();
    glDepthFunc(GL_ALWAYS);    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned width, height;
    App->camera->GetMainCamera()->GetResolution(width, height);

    ModuleProgram::CameraData camera_data;
    // position data is unused on the ui program
    camera_data.pos = float3::zero;
    camera_data.view = float4x4::identity;
    camera_data.proj = float4x4::D3DOrthoProjLH(-1, 1, static_cast<float>(width), static_cast<float>(height));
        
    App->program->UpdateCamera(camera_data);
    BindSquare();
    RecursiveDrawUI(scene->GetRoot(), program);
    UnbindSuare();
    glDepthFunc(GL_LESS);
}

void Hachiko::ModuleUserInterface::RecursiveDrawUI(const GameObject* game_object, Program* program)
{
    ComponentCanvasRenderer* renderer = game_object->GetComponent<ComponentCanvasRenderer>();    

    if (renderer && game_object->IsActive() && renderer->IsActive())
    {
        renderer->Render(program);
    }

    for (const GameObject* child : game_object->children)
    {
        RecursiveDrawUI(child, program);
    }
}

void Hachiko::ModuleUserInterface::CreateSquare()
{
    float positions[] = {
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f // top left 
    };

    unsigned int indices[] = {2, 1, 0, 0, 3, 2};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);    
    
    HE_LOG("QUAD VAO %d %d %d", vao, vbo, ebo);
    glBindVertexArray(0);
}



void Hachiko::ModuleUserInterface::BindSquare()
{
    glBindVertexArray(vao);
}

void Hachiko::ModuleUserInterface::UnbindSuare()
{
    glBindVertexArray(0);
}

void Hachiko::ModuleUserInterface::RemoveSquare()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

