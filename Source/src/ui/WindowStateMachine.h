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

        void showAddNodePopup();
        void showEditNodePopup();

        void addLink();
        void showDeleteLinkPopup();

        void showHelp();

    private:

        //char* trigger = nullptr;
        bool add_node = false;
        bool add_clip = false;

        int node_id = 0;
        bool started = false;
        int link_id = 0;

        //bool editTrigger = false;
        //bool editIT = false;
        bool deleteLink = false;

        ResourceStateMachine* sm = nullptr;
        ImNodesContext* context0 = nullptr;
        ax::NodeEditor::EditorContext* context = nullptr;
    };
} // namespace Hachiko