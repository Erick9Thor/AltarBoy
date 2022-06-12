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

    private:
        void DrawHierarchyTree(const GameObject* game_object);
        bool RecursiveDraw(GameObject* game_object);
        bool DrawGameObject(GameObject* game_object, bool stop);
        bool DragAndDrop(GameObject* game_object);


        GameObject* dragged_object = nullptr;
    };
}
