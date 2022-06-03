#pragma once
#include "Window.h"
#include "resources/ResourceStateMachine.h"

namespace ed = ax::NodeEditor;

namespace Hachiko
{
    class WindowStateMachine final : public Window
    {
    public:

        WindowStateMachine();
        WindowStateMachine(std::string name);

        ~WindowStateMachine() override;

        void Update() override;
        void CleanUp() override;

        void SetStateMachine(ResourceStateMachine* resourceStateMachine);

        void DrawNodes();
        void DrawTransitions();
        void CreateTransitions();

        void ShowContextMenus();
        void ShowAddNodeMenu();
        void ShowNodeMenu();
        void ShowLinkMenu();

        void ShowHelp(); 

    private:

        ImVec2 new_node_pos;
        int nodeId = 0;
        int linkId = 0;

        bool addNode = false;
        bool editTrigger = false;
        bool editIT = false;

        ResourceStateMachine* animation = nullptr;
        ed::EditorContext* context = nullptr;
    };
} // namespace Hachiko