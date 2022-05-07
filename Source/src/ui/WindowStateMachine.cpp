#include "core/hepch.h"
#include "WindowStateMachine.h"

#include "imnodes.h"
#include "resources/ResourceStateMachine.h"

Hachiko::WindowStateMachine::WindowStateMachine() : Window("Hierarchy", true) 
{
    sm.AddState("Idle", "clip1");
    sm.AddState("Walking", "clip2");
    sm.AddState("Running", "clip3");
    sm.AddState("Cry", "clip4");
    sm.AddTransition("Idle", "Walking", "", 1);
    sm.AddTransition("Walking", "Running", "", 1);
    //ImNodes::CreateContext();
}

Hachiko::WindowStateMachine::~WindowStateMachine() 
{
    
}

void Hachiko::WindowStateMachine::Update() 
{
    // Show node editor ------------------------------------------------------------------
    //context = ImNodes::GetCurrentContext();
    //ImNodes::SetNodeGridSpacePos(0, ImVec2(200.0f, 200.0f));
    ImGui::Begin("Node editor");
    ImNodes::BeginNodeEditor();
    id = 0;
    nodes.clear();
    links.clear();
    for (int i = 0; i < sm.states.size(); ++i)
    {
        ImNodes::BeginNode(id++);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(sm.states[i].name.c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(id++);
        ImGui::Text(sm.states[i].clip.c_str());
        ImNodes::EndInputAttribute();

        //std::vector<int> outputs;
        ImNodes::BeginOutputAttribute(id++);
        //ImGui::Text(trigger);
        ImNodes::EndOutputAttribute();
        //outputs.push_back(id - 1);

        ImNodes::EndNode();
        nodes.push_back(Node(sm.states[i].name.c_str(), id - 3, id - 2, id - 1));
    }

    for (int i = 0; i < sm.transitions.size(); ++i)
    {
        int start, end;
        for (int j = 0; j < nodes.size(); ++j)
        {
            if (sm.transitions[i].source == nodes[j].name)
            {
                start = nodes[j].outputIndex;
            }
            if (sm.transitions[i].target == nodes[j].name)
            {
                end = nodes[j].inputIndex;
            }
        }
        ImNodes::Link(i + 1, start, end);
        links.push_back(Link(i + 1, start, end));
    }
    // End of show node editor ------------------------------------------------------------------

    // Show popup to create a node ------------------------------------------------------------------
    const bool openPopup = ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1, false);

    if (!ImGui::IsAnyItemHovered() && openPopup)
    {
        ImGui::OpenPopup("AddNode");
    }

    if (ImGui::BeginPopup("AddNode"))
    {
        if (ImGui::Button(" Add node "))
        {
            addNode = true;
        }

        if (addNode)
        {
            static char nodeName[128] = "";
            const ImGuiInputTextFlags nodeName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Node name ", nodeName, IM_ARRAYSIZE(nodeName), nodeName_input_flags))
            {
                addNode = false;
                sm.AddState(nodeName, "");
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();
    // End of show popup to create a node ------------------------------------------------------------------

    // Show popup edit a node ------------------------------------------------------------------
    if (ImNodes::IsNodeHovered(&nodeId) && ImGui::IsMouseClicked(1, false))
    {
        ImGui::OpenPopup("EditNode");
    }

    if (ImGui::BeginPopup("EditNode"))
    {
        if (ImGui::Button(" Add/change clip "))
        {
            addClip = true;
        }
        else if (ImGui::Button(" Delete clip "))
        {
            deleteClip = true;
        }
        else if (ImGui::Button(" Delete node "))
        {
            deleteNode = true;
        }

        if (addClip)
        {
            static char clipName[128] = "";
            const ImGuiInputTextFlags nodeName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Clip name ", clipName, IM_ARRAYSIZE(clipName), nodeName_input_flags))
            {
                addClip = false;
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (nodes[i].nodeIndex == nodeId)
                    {
                        sm.EditStateClip(nodes[i].name, clipName);
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
        }
        else if (deleteClip)
        {
            deleteClip = false;
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (nodes[i].nodeIndex == nodeId)
                {
                    sm.EditStateClip(nodes[i].name, "");
                }
            }
            ImGui::CloseCurrentPopup();
        }
        else if (deleteNode)
        {
            deleteNode = false;
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (nodes[i].nodeIndex == nodeId)
                {
                    sm.RemoveState(nodes[i].name);
                }
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    // End of show popup to edit a node ------------------------------------------------------------------

    // Add a link ------------------------------------------------------------------
    int start, end;
    if (ImNodes::IsLinkStarted(&start))
    {
        started = true;
    }

    if (started && ImNodes::IsLinkCreated(&start, &end, false))
    {
        std::string startName, endName;
        for (int j = 0; j < nodes.size(); ++j)
        {
            if (nodes[j].outputIndex == start)
            {
                startName = nodes[j].name;
            }
            if (nodes[j].inputIndex == end)
            {
                endName = nodes[j].name;
            }
        }
        sm.AddTransition(startName, endName, "", 3);
    }
    // End of add a link ------------------------------------------------------------------

    // Delete a link ------------------------------------------------------------------
    if (ImNodes::IsLinkHovered(&linkId) && ImGui::IsMouseClicked(1, false))
    {
        ImGui::OpenPopup("DeleteLink");
    }

    if (ImGui::BeginPopup("DeleteLink"))
    {
        if (ImGui::Button(" Delete link "))
        {
            deleteLink = true;
        }

        if (deleteLink)
        {
            deleteLink = false;
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (links[linkId - 1].from == nodes[i].outputIndex)
                {
                    sm.RemoveTransition(nodes[i].name, "");
                    break;
                }
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    // End of delete a link ------------------------------------------------------------------

    ImGui::End();
}

void Hachiko::WindowStateMachine::CleanUp() 
{
    //ImNodes::DestroyContext();
}
