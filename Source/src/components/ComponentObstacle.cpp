#include "core/hepch.h"

#include "ComponentObstacle.h"
#include "Application.h"

Hachiko::ComponentObstacle::ComponentObstacle(GameObject* container) : Component(Type::OBSTACLE, container)
{

}

void Hachiko::ComponentObstacle::DrawGui()
{
    ImGui::PushID(this);
    if (ImGui::CollapsingHeader("Obstacle", ImGuiTreeNodeFlags_DefaultOpen))
    {
    }
    ImGui::PopID();
}

void Hachiko::ComponentObstacle::Save(YAML::Node& node) const {}

void Hachiko::ComponentObstacle::Load(const YAML::Node& node) {}
