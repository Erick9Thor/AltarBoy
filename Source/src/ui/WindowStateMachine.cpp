#include "core/hepch.h"
#include "WindowStateMachine.h"

#include "imnodes.h"
#include "modules/ModuleEditor.h"

Hachiko::WindowStateMachine::WindowStateMachine() : Window("State Machine editor - Press Left Alt + H for help", false)
{
    // ADD example

    sm.AddNode("Idle", "clip1");
    sm.AddNode("Walking", "clip2");
    sm.AddNode("Running", "clip3");
    sm.AddNode("Cry", "clip4");
    sm.AddTransition("Idle", "Walking", "Arrow pressed", 1);
    sm.AddTransition("Idle", "Running", "Shift pressed", 1);
}

Hachiko::WindowStateMachine::~WindowStateMachine()
{
    delete trigger;
}

void Hachiko::WindowStateMachine::Update()
{
    ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin((std::string(ICON_FA_BEZIER_CURVE " ") + name).c_str(), &active))
    {
        ImGui::End();
        return;
    }

    ImNodes::BeginNodeEditor();

    unsigned int id = 0;

    for (int i = 0; i < sm.nodes.size(); ++i)
    {

        ImNodes::BeginNode(i*3);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(sm.nodes[i].name.c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(i * 3 + 1);
        ImGui::Text(sm.nodes[i].clip.c_str());
        ImGui::SameLine();
        ImGui::Text("|");
        ImGui::SameLine();

        //TODO: if (sm.clips[sm.FindClip(sm.states[i].clip)].loop)
        if (false)
        {
            ImGui::Text("looping");
        }
        else
        {
            ImGui::Text("not looping");
        }

        ImNodes::EndInputAttribute();
        /*
        std::vector<Hachiko::ResourceStateMachine::Transition> transitionsFromState = sm.FindTransitionsFromNode(node.name.c_str());
        for (int j = 0; j < transitionsFromState.size(); ++j)
        {
            ImNodes::BeginOutputAttribute(id++);
            ImGui::Text(transitionsFromState[j].trigger.c_str());
            ImGui::SameLine();
            ImGui::Text("|");
            ImGui::SameLine();
            ImGui::Text(std::to_string(transitionsFromState[j].interpolationTime).c_str());
            ImNodes::EndOutputAttribute();
        }
        */
        // OUTPUT BY TRIGGER
        ImNodes::BeginOutputAttribute(i * 3 + 2);
        ImGui::Text("");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
    }

    id = 0;
    for (const Hachiko::ResourceStateMachine::Transition& transition : sm.transitions)
    {
        ImNodes::Link(id, sm.FindNode(transition.source) * 3 + 2, sm.FindNode(transition.target) * 3 + 1);
        ++id;
    }


    showCreatePopUp();

    ImNodes::EndNodeEditor();

    // showEditPopUp();

    showAddLink();
    // showDeleteLink();
    // showHelpLink();
    ImGui::End();
}


void Hachiko::WindowStateMachine::showCreatePopUp() 
{
    const bool openPopup = ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1, false);

    if (!ImGui::IsAnyItemHovered() && openPopup)
    {
        ImGui::OpenPopup("AddNode");
    }

    if (ImGui::BeginPopup("AddNode"))
    {
        if (ImGui::Button(" Add node "))
        {
            add_node = true;
        }

        if (add_node)
        {
            static char nodeName[128] = "";
            const ImGuiInputTextFlags nodeName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Node name ", nodeName, IM_ARRAYSIZE(nodeName), nodeName_input_flags))
            {
                add_node = false;
                sm.AddNode(nodeName, "");
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
}

void Hachiko::WindowStateMachine::showEditPopUp() 
{
    if (ImNodes::IsNodeHovered(&node_id) && ImGui::IsMouseClicked(1, false))
    {
        ImGui::OpenPopup("EditNode");
    }

    if (ImGui::BeginPopup("EditNode"))
    {
        if (ImGui::Button(" Add/change clip "))
        {
            add_clip = true;
        }
        /*
            else if (ImGui::Button(" edit looping "))
            {
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (nodes[i].nodeIndex == nodeId)
                    {
                        sm.EditClipLoop(nodes[i].name, !sm.clips[sm.FindClip(sm.states[i].clip)].loop);
                    }
                }
                ImGui::CloseCurrentPopup();
            }*/
        else if (ImGui::Button(" Delete clip "))
        {
            for (int i = 0; i < sm.nodes.size(); ++i)
            {
                if (sm.nodes[i].nodeIndex == node_id)
                {
                    // sm.EditStateClip(sm.nodes[i].name, "");
                }
            }
            ImGui::CloseCurrentPopup();
        }
        else if (ImGui::Button(" Delete node "))
        {
            for (int i = 0; i < sm.nodes.size(); ++i)
            {
                if (sm.nodes[i].nodeIndex == node_id)
                {
                    // sm.RemoveState(nodes[i].name);
                }
            }
            ImGui::CloseCurrentPopup();
        }

        if (add_clip)
        {
            static char clipName[128] = "";
            const ImGuiInputTextFlags clipName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Clip name ", clipName, IM_ARRAYSIZE(clipName), clipName_input_flags))
            {
                add_clip = false;
                for (int i = 0; i < sm.nodes.size(); ++i)
                {
                    if (sm.nodes[i].nodeIndex == node_id)
                    {
                        // sm.EditStateClip(sm.nodes[i].name, clipName);
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
        }

        ImGui::EndPopup();
    }
}

void Hachiko::WindowStateMachine::showAddLink() 
{
    int start, end;
    if (ImNodes::IsLinkStarted(&start))
    {
        started = true;
    }

    if (started && ImNodes::IsLinkCreated(&start, &end, false))
    {
        sm.AddTransition(sm.nodes[(start - 2) / 3].name, sm.nodes[(end - 1) / 3].name, "", 0);
    }
}

void Hachiko::WindowStateMachine::showDeleteLink() 
{
    /* if (ImNodes::IsLinkHovered(&linkId) && ImGui::IsMouseClicked(1, false))
    {
        ImGui::OpenPopup("EditLink");
    }

    if (ImGui::BeginPopup("EditLink"))
    {
        if (ImGui::Button(" Edit trigger "))
        {
            editTrigger = true;
            editIT = false;
        }
        else if (ImGui::Button(" Edit interpolation time "))
        {
            editIT = true;
            editTrigger = false;
        }
        else if (ImGui::Button(" Delete link "))
        {
            std::string startName, endName;
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (startName == "")
                {
                    for (int j = 0; j < nodes[i].outputIndex.size(); ++j)
                    {
                        if (links[linkId].from == nodes[i].outputIndex[j])
                        {
                            startName = nodes[i].name;
                            break;
                        }
                    }
                }
                if (links[linkId].to == nodes[i].inputIndex)
                {
                    endName = nodes[i].name;
                    if (startName != "")
                    {
                        break;
                    }
                }
            }
            sm.RemoveTransitionWithTarget(startName, endName);
            ImGui::CloseCurrentPopup();
        }

        if (editTrigger)
        {
            static char newTrigger[128] = "";
            const ImGuiInputTextFlags editTrigger_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Edit trigger", newTrigger, IM_ARRAYSIZE(newTrigger), editTrigger_input_flags))
            {
                editTrigger = false;
                std::string startName, endName;
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (startName == "")
                    {
                        for (int j = 0; j < nodes[i].outputIndex.size(); ++j)
                        {
                            if (links[linkId].from == nodes[i].outputIndex[j])
                            {
                                startName = nodes[i].name;
                                break;
                            }
                        }
                    }
                    if (links[linkId].to == nodes[i].inputIndex)
                    {
                        endName = nodes[i].name;
                        if (startName != "")
                        {
                            break;
                        }
                    }
                }
                sm.EditTransitionTrigger(startName, endName, newTrigger);
                ImGui::CloseCurrentPopup();
            }
        }

        if (editIT)
        {
            static char newIT[128] = "";
            const ImGuiInputTextFlags editIT_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Edit interpolation time", newIT, IM_ARRAYSIZE(newIT), editIT_input_flags))
            {
                editIT = false;
                std::string startName, endName;
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (startName == "")
                    {
                        for (int j = 0; j < nodes[i].outputIndex.size(); ++j)
                        {
                            if (links[linkId].from == nodes[i].outputIndex[j])
                            {
                                startName = nodes[i].name;
                                break;
                            }
                        }
                    }
                    if (links[linkId].to == nodes[i].inputIndex)
                    {
                        endName = nodes[i].name;
                        if (startName != "")
                        {
                            break;
                        }
                    }
                }
                sm.EditTransitionInterpolationTime(startName, endName, std::stoi(newIT));
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }*/
}

void Hachiko::WindowStateMachine::showHelpLink() 
{
    if (ImGui::IsKeyDown(SDL_SCANCODE_LALT) && ImGui::IsKeyPressed(SDL_SCANCODE_H, false))
    {
        ImGui::OpenPopup("Help");
    }

    if (ImGui::BeginPopup("Help"))
    {
        ImGui::Text("Controls:");
        ImGui::BulletText("Use the mouse wheel to move around the editor");
        ImGui::Text("   or mouse left-clicking at the mini-map.");
        ImGui::Separator();
        ImGui::BulletText("Mouse right-clicking to add a node.");
        ImGui::Separator();
        ImGui::BulletText("Mouse right-clicking hovering a node to edit it.");
        ImGui::Separator();
        ImGui::BulletText("Mouse left-clicking hovering a pin to start a link");
        ImGui::Text("   and release it in another pin to create it.");
        ImGui::Separator();
        ImGui::BulletText("Mouse right-clicking hovering a link to edit it.");
        ImGui::Separator();
        ImGui::Text("Node legend:");
        ImGui::Text("                  Node name");
        ImGui::Text("                Clip  | is it looping?");
        ImGui::Text("              Trigger | interpolation time");

        ImGui::EndPopup();
    }
}

void Hachiko::WindowStateMachine::CleanUp()
{
    ImNodes::DestroyContext();
}