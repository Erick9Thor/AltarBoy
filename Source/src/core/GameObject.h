#pragma once

#include "utils/UUID.h"
#include "core/serialization/Serializable.h"
#include "components/Component.h"

namespace Hachiko
{
    class ComponentTransform;
    class ComponentCamera;
    class Program;
    class Scene;

    class GameObject final : public Serializable
    {
        friend class Component;

    public:
        GameObject(const char* name = "Unnamed");
        GameObject(GameObject* parent,
                   const float4x4& transform, 
                   const char* name = "Unnamed", 
                   UID uid = UUID::GenerateUID());
        GameObject(GameObject* parent,
                   const char* name = "Unnamed",
                   UID uid = UUID::GenerateUID(),
                   const float3& translation = float3::zero,
                   const Quat& rotation = Quat::identity,
                   const float3& scale = float3::one);
        virtual ~GameObject();

        void SetNewParent(GameObject* new_parent);

        void AddComponent(Component* component);
        bool AttemptRemoveComponent(Component* component);

        Component* CreateComponent(Component::Type type);
        void RemoveChild(GameObject* gameObject);

        //void Destroy();
        void Start();
        void Stop();
        void Update();
        void DrawAll(ComponentCamera* camera, Program* program) const;
        void Draw(ComponentCamera* camera, Program* program) const;
        void DrawStencil(ComponentCamera* camera, Program* program);

        void SetActive(bool set_active);

        [[nodiscard]] bool IsActive() const
        {
            return active;
        }

        void OnTransformUpdated();

        void DebugDrawAll();
        void DebugDraw() const;
        void DrawBoundingBox() const;
        void UpdateBoundingBoxes();

        [[nodiscard]] UID GetID() const
        {
            return uid;
        }

        void SetID(const UID new_id) 
        {
            uid = new_id;
        }

        void Save(YAML::Node& node) const;
        void Load(const YAML::Node& node);

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
        RetComponent* GetComponent() const
        {
            const Component::Type type = RetComponent::GetType();
            for (auto& component : components)
            {
                if (component->GetType() == type)
                    return static_cast<RetComponent*>(component);
            }
            return nullptr;
        }

        [[nodiscard]] const std::string& GetName() const
        {
            return name;
        }

        void SetName(const std::string& new_name)
        {
            name = new_name;
        }

        void Enable()
        {
            active = true;
        }

        void Disable()
        {
            active = false;
        }

        Scene* scene_owner = nullptr;
        GameObject* parent = nullptr;
        std::vector<GameObject*> children;

    public:
        bool active = true;
        std::string name;
        bool started = false;
        std::vector<Component*> components;
        ComponentTransform* transform = nullptr;

        AABB aabb;
        OBB obb;

        UID uid = 0;
    };
}
