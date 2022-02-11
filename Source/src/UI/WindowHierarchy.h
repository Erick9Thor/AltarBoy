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
        void DrawHierarchyTree(GameObject* root);
        void DrawGOChilds(GameObject* root);
        void DrawGameObject(GameObject* go);

        GameObject* dragged_object = nullptr;
    };
}
