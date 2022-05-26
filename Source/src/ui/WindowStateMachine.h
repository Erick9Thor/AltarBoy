#pragma once
#include "Window.h"
#include "resources/ResourceStateMachine.h"

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

        void SetStateMachine(ResourceStateMachine& resourceStateMachine);

        void DrawNodes();
        void DrawTransitions();

        void ShowAddNodePopup();
        void ShowContextMenus();
        void ShowNodeMenu();
        void ShowLinkMenu();






        void showEditNodePopup();
        void addLink();
        void showDeleteLinkPopup();






        void ShowHelp(); 

    private:

        //char* trigger = nullptr;
        bool addNode = false;
        bool add_clip = false;

        int node_id = 0;
        bool started = false;
        int link_id = 0;

        //bool editTrigger = false;
        //bool editIT = false;
        bool deleteLink = false;

        int context_node = 0;
        int context_link = 0;
        ImVec2 new_node_pos;
        ax::NodeEditor::PinId new_node_pin = 0;

        ResourceStateMachine* stateMachine = nullptr;
        ax::NodeEditor::EditorContext* context = nullptr;
    };
} // namespace Hachiko