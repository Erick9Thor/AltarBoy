#pragma once

#include "utils/UUID.h"
#include "utils/JsonFormatterValue.h"

#if defined(HACHIKO_API)
// Do Nothing
#elif defined(_MSC_VER)
#define HACHIKO_API __declspec(dllexport)
#endif

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class Program;

    class HACHIKO_API Component
    {
    public:
        enum class Type
        {
            NONE = 0,
            TRANSFORM = 1,
            MESH = 2,
            MATERIAL = 3,
            CAMERA = 4,
            DIRLIGHT = 5,
            POINTLIGHT = 6,
            SPOTLIGHT = 7,
            CANVAS = 8,
            CANVAS_RENDERER = 9,
            TRANSFORM_2D = 10,
            IMAGE = 11,
            BUTTON = 12,
            PROGRESS_BAR = 13,
            SCRIPT = 14,
            UNKNOWN
        };

        Component(const Type type, GameObject* container) :
            game_object(container),
            type(type) {}

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

        [[nodiscard]] bool IsActive() const
        {
            return active;
        }

        virtual bool CanBeRemoved() const;
        virtual bool HasDependentComponents(GameObject* game_object) const;

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