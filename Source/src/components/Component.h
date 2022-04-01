#pragma once

#include "utils/UUID.h"
#include "utils/JsonFormatterValue.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class Program;

    class Component
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
            SCRIPT,
            UNKNOWN
        };

        Component(const Type type, GameObject* container) :
            game_object(container),
            type(type) {}

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

        virtual void Save(JsonFormatterValue j_component) const {}

        virtual void Load(JsonFormatterValue j_component) {}

    protected:
        void OverrideUID(UID new_uid) 
        {
            uid = new_uid;
        }

    protected:
        GameObject* game_object = nullptr;
        bool active = true;
        Type type = Type::UNKNOWN;

    private:
        UID uid = 0;
    };
}
