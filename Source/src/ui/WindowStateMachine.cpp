#include "core/hepch.h"
#include "WindowStateMachine.h"

#include "imnodes.h"
#include "modules/ModuleEditor.h"

Hachiko::WindowStateMachine::WindowStateMachine() : Window("State Machine editor - Press Left Alt + H for help", true) 
{
    sm.AddState("Idle", "clip1");
    sm.AddState("Walking", "clip2");
    sm.AddState("Running", "clip3");
    sm.AddState("Cry", "clip4");
    sm.AddTransition("Idle", "Walking", "Arrow pressed", 1);
    sm.AddTransition("Idle", "Running", "Shift pressed", 1);
    //ImNodes::CreateContext();
}

Hachiko::WindowStateMachine::~WindowStateMachine() 
{
    
}

void Hachiko::WindowStateMachine::Update() 
{
    ImGui::SetNextWindowDockID(App->editor->dock_main_id, ImGuiCond_FirstUseEver);
    // Show node editor ------------------------------------------------------------------
    //context = ImNodes::GetCurrentContext();
    if (ImGui::Begin((std::string(ICON_FA_BEZIER_CURVE " ") + name).c_str(), &active))
    {
        ImNodes::BeginNodeEditor();
        int id = 0;
        nodes.clear();
        links.clear();
        for (int i = 0; i < sm.states.size(); ++i)
        {
            int initId = id;
            ImNodes::BeginNode(id++);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(sm.states[i].name.c_str());
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(id++);
            ImGui::Text(sm.states[i].clip.c_str());
            ImGui::SameLine();
            ImGui::Text("|");
            ImGui::SameLine();
            /*
            if (sm.clips[sm.FindClip(sm.states[i].clip)].loop)
            {
                ImGui::Text("looping");
            }
            else
            {
                ImGui::Text("not looping");
            }
            */
            ImNodes::EndInputAttribute();

            std::vector<int> outputs;
            std::vector<Hachiko::ResourceStateMachine::Transition> transitionsFromState = sm.FindTransitionsFromState(sm.states[i].name.c_str());
            for (int j = 0; j < transitionsFromState.size(); ++j)
            {
                outputs.push_back(id);
                ImNodes::BeginOutputAttribute(id++);
                ImGui::Text(transitionsFromState[j].trigger.c_str());
                ImGui::SameLine();
                ImGui::Text("|");
                ImGui::SameLine();
                ImGui::Text(std::to_string(transitionsFromState[j].interpolationTime).c_str());
                ImNodes::EndOutputAttribute();
            }
            outputs.push_back(id);
            ImNodes::BeginOutputAttribute(id++);
            ImGui::Text("");
            ImNodes::EndOutputAttribute();

            //ImNodes::SetNodeGridSpacePos(initId, ImVec2(id * 10.0f, id * 10.0f));
            //ImNodes::SetNodeDraggable(initId, true);

            ImNodes::EndNode();
            nodes.push_back(Node(sm.states[i].name.c_str(), initId, initId + 1, outputs, transitionsFromState));
        }

        id = 0;
        for (int i = 0; i < nodes.size(); ++i)
        {
            for (int j = 0; j < nodes[i].transitionsFromNode.size(); ++j)
            {
                for (int k = 0; k < nodes.size(); ++k)
                {
                    if (i != k && nodes[k].name == nodes[i].transitionsFromNode[j].target)
                    {
                        ImNodes::Link(id, nodes[i].outputIndex[j], nodes[k].inputIndex);
                        links.push_back(Link(id, nodes[i].outputIndex[j], nodes[k].inputIndex));
                        ++id;
                    }
                }
            }
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
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (nodes[i].nodeIndex == nodeId)
                    {
                        sm.EditStateClip(nodes[i].name, "");
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            else if (ImGui::Button(" Delete node "))
            {
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (nodes[i].nodeIndex == nodeId)
                    {
                        sm.RemoveState(nodes[i].name);
                    }
                }
                ImGui::CloseCurrentPopup();
            }

            if (addClip)
            {
                static char clipName[128] = "";
                const ImGuiInputTextFlags clipName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
                if (ImGui::InputText(" Clip name ", clipName, IM_ARRAYSIZE(clipName), clipName_input_flags))
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
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (startName == "")
                {
                    for (int j = 0; j < nodes[i].outputIndex.size(); ++j)
                    {
                        if (start == nodes[i].outputIndex[j])
                        {
                            startName = nodes[i].name;
                        }
                    }
                }
                if (end == nodes[i].inputIndex)
                {
                    endName = nodes[i].name;
                }
            }
            sm.AddTransition(startName, endName, "Set trigger", 0);
        }
        // End of add a link ------------------------------------------------------------------

        // Delete a link ------------------------------------------------------------------
        if (ImNodes::IsLinkHovered(&linkId) && ImGui::IsMouseClicked(1, false))
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
        }
        // End of delete a link ------------------------------------------------------------------

        // Help section ------------------------------------------------------------------
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
        // End of help section ------------------------------------------------------------------

        ImGui::End();
    }
}

void Hachiko::WindowStateMachine::CleanUp() 
{
    //ImNodes::DestroyContext();
}
