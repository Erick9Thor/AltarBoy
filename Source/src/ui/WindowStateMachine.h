#pragma once
#include "Window.h"
#include "resources/ResourceStateMachine.h"

namespace Hachiko
{
    class WindowStateMachine final : public Window
    {
    public:
        

        WindowStateMachine();
        ~WindowStateMachine() override;

        void Update() override;
        void CleanUp() override;

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

        ResourceStateMachine sm = ResourceStateMachine(1);
        ImNodesContext* context = nullptr;
    };
} // namespace Hachiko