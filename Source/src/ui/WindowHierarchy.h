#pragma once
#include "Window.h"

namespace Hachiko
{
    class GameObject;

    class WindowHierarchy final : public Window
    {
    public:
        WindowHierarchy();
        ~WindowHierarchy() override;
        void Update() override;

        void CleanUp() override;

    private:
        void DrawHierarchyTree(const GameObject* game_object);
        void DrawChildren(const GameObject* game_object);
        void DrawGameObject(GameObject* game_object);
        bool DragAndDrop(GameObject* game_object);
        GameObject* dragged_object = nullptr;
    };
}
