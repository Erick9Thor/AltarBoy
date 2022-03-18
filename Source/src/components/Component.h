#pragma once

#include "core/serialization/Serializable.h"
#include "utils/UUID.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class Program;

    class Component : public Serializable
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
            UNKNOWN
        };

        Component(const Type type, GameObject* container, UID id = 0) :
            game_object(container),
            type(type),
            uid(id)
        {}

        virtual ~Component() = default;

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

        bool IsActive()
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

    protected:
        GameObject* game_object = nullptr;
        bool active = true;
        Type type = Type::UNKNOWN;

    private:
        UID uid = 0;
    };
}
