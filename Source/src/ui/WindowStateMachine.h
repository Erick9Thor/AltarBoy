#pragma once
#include "Window.h"
#include "resources/ResourceStateMachine.h"
#include "imnodes.h"

namespace Hachiko
{
    class WindowStateMachine final : public Window
    {
    public:
        struct Node
        {
            std::string name;
            int nodeIndex;
            int inputIndex;
            int outputIndex;
            //std::vector<int> outputIndex;

            Node() {};
            Node(const std::string& name, int nodeIndex, int inputIndex, int outputIndex) : name(name), nodeIndex(nodeIndex), inputIndex(inputIndex), outputIndex(outputIndex) {};
            //Node(const std::string& name, int nodeIndex, int inputIndex, std::vector<int> outputIndex) : name(name), nodeIndex(nodeIndex), inputIndex(inputIndex), outputIndex(outputIndex) {};
        };
        struct Link
        {
            int id;
            int from;
            int to;

            Link() {};
            Link(int id, int from, int to) : id(id), from(from), to(to) {};
        };

        WindowStateMachine();
        ~WindowStateMachine() override;

        void Update() override;
        void CleanUp() override;

    private:
        std::vector<Node> nodes;
        std::vector<Link> links;

        int id = 0;
        char* trigger;
        bool addNode = false;
        bool addClip = false;
        bool deleteClip = false;
        bool deleteNode = false;
        int nodeId = 0;
        bool started = false;
        int linkId = 0;
        bool deleteLink = false;
        ResourceStateMachine sm = ResourceStateMachine(1);
        ImNodesContext* context = nullptr;
    };
} // namespace Hachiko