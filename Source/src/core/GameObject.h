#pragma once
#include "MathGeoLib.h"

#include <vector>
#include "utils/UUID.h"
#include "components/Component.h"

#include "modules/ModuleEvent.h"
#include "events/Event.h"

namespace Hachiko
{
    class JsonFormatterValue;
    class ComponentTransform;
    class ComponentCamera;
    class Program;
    class Scene;

    class GameObject final
    {
        friend class Component;

    public:
        GameObject(const char* name = "Unnamed");
        GameObject(GameObject* parent, const float4x4& transform, const char* name = "Unnamed", UID uid = UUID::GenerateUID());
        GameObject(GameObject* parent,
                   const char* name = "Unnamed",
                   UID uid = UUID::GenerateUID(),
                   const float3& translation = float3::zero,
                   const Quat& rotation = Quat::identity,
                   const float3& scale = float3::one);
        virtual ~GameObject();

        void SetNewParent(GameObject* new_parent);

        void AddComponent(Component* component);
        void RemoveComponent(Component* component);

        Component* CreateComponent(Component::Type type);
        void RemoveChild(GameObject* gameObject);

        //void Destroy();
        void Update();
        void DrawAll(ComponentCamera* camera, Program* program) const;
        void Draw(ComponentCamera* camera, Program* program) const;
        void DrawStencil(ComponentCamera* camera, Program* program);

        bool IsActive() const
        {
            return active;
        }

        void OnTransformUpdated();

        void DebugDrawAll();
        void DebugDraw() const;
        void DrawBoundingBox() const;
        void UpdateBoundingBoxes();

        UID getUID() const
        {
            return uid;
        }

        void Save(JsonFormatterValue j_gameObject) const;
        void Load(JsonFormatterValue j_gameObject);

        [[nodiscard]] const OBB& GetOBB() const
        {
            return obb;
        }

        const AABB& GetAABB()
        {
            return aabb;
        }

        [[nodiscard]] const std::vector<Component*>& GetComponents() const
        {
            return components;
        }

        [[nodiscard]] ComponentTransform* GetTransform() const 
        {
            return transform;
        }

        template<typename RetComponent>
        RetComponent* GetComponent()
        {
            const Component::Type type = RetComponent::GetType();
            for (auto& component : components)
            {
                if (component->GetType() == type)
                    return static_cast<RetComponent*>(component);
            }
            return nullptr;
        }

        std::string name;
        Scene* scene_owner = nullptr;
        GameObject* parent = nullptr;
        std::vector<GameObject*> children;

        bool active = true;

    private:
        std::vector<Component*> components;
        ComponentTransform* transform = nullptr;

        AABB aabb;
        OBB obb;

        UID uid = 0;
    };
}
