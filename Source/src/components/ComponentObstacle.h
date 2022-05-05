#pragma once

#include "components/Component.h"
#include "Globals.h"
#include "DetourTileCache/DetourTileCache.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform2D;

    class ComponentObstacle : public Component
    {
    public:
        ComponentObstacle(GameObject* container);
        ~ComponentObstacle() override = default;

        static Type GetType();

        void DrawGui() override;
        
        // TODO: Add debugdraw

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

    private:
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentObstacle::GetType()
{
    return Type::OBSTACLE;
}
