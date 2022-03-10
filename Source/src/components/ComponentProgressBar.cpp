#include "core/hepch.h"
#include "ComponentProgressBar.h"

Hachiko::ComponentProgressBar::ComponentProgressBar(GameObject* container) : Component(Type::PROGRESS_BAR, container) 
{
	// Get backgroundID;
	// Get foregroundID;
	// Get background;
	// Get foreground;
}

void Hachiko::ComponentProgressBar::Update() 
{
    if (background == nullptr || foreground == nullptr)
    {
        // Find background and foreground in gameObjects childs
    }

    if (background != nullptr && foreground != nullptr)
    {
        // Get background_transform2D;
        // Get foreground_transform2D;
    }
    else
    {
        return;
    }
}

void Hachiko::ComponentProgressBar::DrawGui()
{
    ImGui::DragFloat("Min", &min, 1.0f, -inf, max - 1);
    ImGui::DragFloat("Max", &max, 1.0f, min + 1, inf);
    ImGui::DragFloat("Filled", &filled, 1.0f, min, max);

    ImGui::Combo("Filling Direction", &direction_index, filling_directions, IM_ARRAYSIZE(filling_directions));
    direction = static_cast<FillingDirection>(direction_index);
}
