#include "core/hepch.h"
#include "ComponentCanvasRenderer.h"
#include "ComponentImage.h"
#include "ComponentButton.h"
#include "ComponentCanvas.h"
#include "ComponentTransform2D.h"

#include "Program.h"

Hachiko::ComponentCanvasRenderer::ComponentCanvasRenderer(GameObject* container) : Component(Type::CANVAS_RENDERER, container) {}

void Hachiko::ComponentCanvasRenderer::DrawGui()
{
    if (ImGui::CollapsingHeader("Canvas Renderer", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Active: %d", active);
    }
}

bool Hachiko::ComponentCanvasRenderer::HasDependentComponents(GameObject* game_object) const
{
    // TODO: Add all UI components that depend on renderer
    bool any_ui_element = game_object->GetComponent<ComponentImage>() || game_object->GetComponent<ComponentButton>();
    return any_ui_element;
}

Hachiko::ComponentCanvas* Hachiko::ComponentCanvasRenderer::FindClosestParentCanvas() const
{
    ComponentCanvas* canvas = nullptr;
    GameObject* parent = game_object->parent;
    while (parent)
    {
        canvas = parent->GetComponent<ComponentCanvas>();
        if (canvas)
        {
            break;
        }
        parent = parent->parent;
    }
    return canvas;
}

void Hachiko::ComponentCanvasRenderer::Render(Program* program) const
{
    ComponentCanvas* canvas = FindClosestParentCanvas();
    ComponentTransform2D* transform = game_object->GetComponent<ComponentTransform2D>();

    if (canvas && transform)
    {
        // TODO: Make draw calls for all ui components of current game object
        ComponentImage* image = game_object->GetComponent<ComponentImage>();
        if (image && image->IsActive())
        {
            image->Draw(transform, program);
        }
        
    }
}
