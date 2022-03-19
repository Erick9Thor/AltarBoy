#pragma once
#include "Module.h"
#include "MathGeoLib.h"

#include "ui/WindowScene.h"

namespace Hachiko
{
    class ModuleUserInterface : public Module
    {
    public:
        ModuleUserInterface();
        ~ModuleUserInterface() override;

        bool Init() override;
        UpdateStatus Update(float delta) override;
        bool CleanUp() override;

        void DrawUI(const Scene* scene);
        void RecursiveDrawUI(const GameObject* game_object, Program* program);
        void RecursiveCheckMousePos(const GameObject* game_object, const float2& mouse_pos);

        void CreateSquare();
        void BindSquare();
        void UnbindSuare();
        void RemoveSquare();

    private:
        unsigned vbo = 0, ebo = 0, vao = 0;
    };
}
