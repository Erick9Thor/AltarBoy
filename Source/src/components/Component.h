#pragma once

#include "core/serialization/ISerializable.h"
#include "utils/UUID.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class Program;

    class Component : public ISerializable
    {
    public:
        enum class Type
        {
            NONE,
            TRANSFORM,
            MESH,
            MATERIAL,
            CAMERA,
            DIRLIGHT,
            POINTLIGHT,
            SPOTLIGHT,
            CANVAS,
            CANVAS_RENDERER,
            TRANSFORM_2D,
            IMAGE,
            BUTTON,
            PROGRESS_BAR,
            UNKNOWN
        };

        Component(const Type type, GameObject* container, UID id = 0) :
            game_object(container),
            type(type),
            uid(id)
        {
            if (!uid)
            {
                uid = UUID::GenerateUID();
            }
        }

        virtual ~Component() = default;

        virtual void Start() { }
        virtual void Update() { }

        virtual void OnTransformUpdated() {}

        [[nodiscard]] Type GetType() const
        {
            return type;
        }

        [[nodiscard]] UID GetID() const
        {
            return uid;
        }

        void SetID(const UID new_uid)
        {
            uid = new_uid;
        }

        void Enable()
        {
            active = true;
        }

        void Disable()
        {
            active = false;
        }

        [[nodiscard]] bool IsActive() const
        {
            return active;
        }

        [[nodiscard]] const GameObject* GetGameObject() const
        {
            return game_object;
        }

        GameObject* GetGameObject()
        {
            return game_object;
        }

        void SetGameObject(GameObject* container)
        {
            game_object = container;
        }

        virtual void DrawGui() {}

        virtual void Draw(ComponentCamera* camera, Program* program) {}

        virtual void DebugDraw() {}

        virtual void Save(YAML::Node& node) const {}

        virtual void Load(const YAML::Node& node) {}

        virtual bool CanBeRemoved() const;
        virtual bool HasDependentComponents(GameObject* game_object) const;

    protected:
        GameObject* game_object = nullptr;
        bool active = true;
        Type type = Type::UNKNOWN;

    private:
        UID uid = 0;
    };
}
