#include "core/hepch.h"
#include "WindowStateMachine.h"

#include "imgui_node_editor.h"
#include "modules/ModuleEditor.h"

using namespace ax;

Hachiko::WindowStateMachine::WindowStateMachine() : Window("State Machine editor - Press Left Alt + H for help", false)
{
    

}

Hachiko::WindowStateMachine::WindowStateMachine(std::string name) : Window(std::string(name + std::string(" State Machine editor - Press Left Alt + H for help")).c_str(), false)
{
    context = NodeEditor::CreateEditor();
}

Hachiko::WindowStateMachine::~WindowStateMachine()
{
    stateMachine = nullptr;
    NodeEditor::DestroyEditor(context);
    //delete context;
    //delete context0;
    //delete trigger;
}

void Hachiko::WindowStateMachine::Update()
{
    
    ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin((std::string(ICON_FA_BEZIER_CURVE " ") + "StateMachine").c_str(), &active))
    {
        ImGui::End();
        return;
    }
    
    /*
    ImNodes::BeginNodeEditor();

    unsigned int id = 0;

    for (int i = 0; i < sm->nodes.size(); ++i)
    {

        ImNodes::BeginNode(i*3);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(sm->nodes[i].name.c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(i * 3 + 1);
        ImGui::Text(sm->nodes[i].clip.c_str());
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

        ImNodes::BeginOutputAttribute(i * 3 + 2);
        ImGui::Text("");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
    }

    //id = 0;
    for (const Hachiko::ResourceStateMachine::Transition& transition : sm->transitions)
    {
        int sourceID = sm->FindNode(transition.source);
        int targetID = sm->FindNode(transition.target);
        id = sourceID * 100 + targetID;
        ImNodes::Link(id, sourceID * 3 + 2, targetID * 3 + 1);
        //ImNodes::Link(id, sm.FindNode(transition.source) * 3 + 2, sm.FindNode(transition.target) * 3 + 1);
        //++id;
    }

    showAddNodePopup();

    ImNodes::EndNodeEditor();
    

    showEditNodePopup();

    addLink();
    showDeleteLinkPopup();
    */


    NodeEditor::SetCurrentEditor(context);
    NodeEditor::Begin("State Machine Editor", ImVec2(0.0, 0.0f)); // TODO: Revise why this causes memory leaks
    
    DrawNodes();
    DrawTransitions();
    CreateTransitions();

    NodeEditor::Suspend();
    ShowContextMenus();
    ShowAddNodeMenu();
    ShowNodeMenu();
    ShowLinkMenu();
    OpenPopups();
    NodeEditor::Resume();
   
    ShowHelp();

    NodeEditor::End();
    NodeEditor::SetCurrentEditor(nullptr);

    ImGui::End();
}

void Hachiko::WindowStateMachine::DrawNodes()
{
    for (int i = 0; i < stateMachine->nodes.size(); ++i)
    {
        NodeEditor::PushStyleColor(NodeEditor::StyleColor_PinRect, ImColor(60, 180, 255, 150));
        NodeEditor::PushStyleColor(NodeEditor::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

        int id = i * 3;
        NodeEditor::BeginNode(id + 1);
        
        ImVec2 size = NodeEditor::GetNodeSize(id + 1);

        ImGui::Indent(1.0);
        if (stateMachine->nodes[i].name.size() <= 7)
        {
            ImGui::Text("");
            float offset = (8 - stateMachine->nodes[i].name.size()) * 0.05;
            ImGui::SameLine(size.x * offset);
        }
        ImGui::TextColored(ImVec4(255, 255, 0, 255), stateMachine->nodes[i].name.c_str());

        ImGui::Text(stateMachine->nodes[i].clip.c_str());
        
        if (!stateMachine->clips.empty())
        {
            ImGui::SameLine();
            if (stateMachine->clips[stateMachine->FindClip(stateMachine->nodes[i].clip.c_str())].loop)
            {
                ImGui::Text(" | looping");
            }
            else
            {
                ImGui::Text(" | not looping");
            }
        }
        
        // In Pin
        NodeEditor::PushStyleVar(NodeEditor::StyleVar_PinArrowSize, 8.0f);
        NodeEditor::PushStyleVar(NodeEditor::StyleVar_PinArrowWidth, 8.0f);
        NodeEditor::PushStyleVar(NodeEditor::StyleVar_PinRadius, 10.0f);
        NodeEditor::PushStyleVar(NodeEditor::StyleVar_TargetDirection, ImVec2(0.0f, 0.0f));
        NodeEditor::BeginPin(id + 2, NodeEditor::PinKind::Input);
        ImGui::Text("In");
        NodeEditor::EndPin();
        NodeEditor::PopStyleVar(4);

        // Out Pin
        if (size.x > 80)
        {
            ImGui::SameLine(size.x - 40);
        }
        else
        {
            ImGui::SameLine(40);
        }
        NodeEditor::PushStyleVar(NodeEditor::StyleVar_PinArrowSize, 0.0f);
        NodeEditor::PushStyleVar(NodeEditor::StyleVar_PinArrowWidth, 0.0f);
        NodeEditor::PushStyleVar(NodeEditor::StyleVar_TargetDirection, ImVec2(0.0f, 0.0f));
        NodeEditor::BeginPin(id + 3, NodeEditor::PinKind::Output);
        ImGui::Text("Out");
        NodeEditor::EndPin();

        NodeEditor::EndNode();
    }
}

void Hachiko::WindowStateMachine::DrawTransitions()
{
    for (const Hachiko::ResourceStateMachine::Transition& transition : stateMachine->transitions)
    {
        int sourceID = stateMachine->FindNode(transition.source);
        int targetID = stateMachine->FindNode(transition.target);
        int linkID = sourceID * 100 + targetID;
        NodeEditor::Link(linkID, sourceID * 3 + 3, targetID * 3 + 2);
    }
}

void Hachiko::WindowStateMachine::CreateTransitions() 
{
    if (NodeEditor::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        auto showLabel = [](const char* label, ImColor color) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
            auto size = ImGui::CalcTextSize(label);

            auto padding = ImGui::GetStyle().FramePadding;
            auto spacing = ImGui::GetStyle().ItemSpacing;

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

            auto rectMin = ImGui::GetCursorScreenPos() - padding;
            auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

            auto drawList = ImGui::GetWindowDrawList();
            drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
            ImGui::TextUnformatted(label);
        };

        NodeEditor::PinId startPinId = 0, endPinId = 0;
        if (NodeEditor::QueryNewLink(&startPinId, &endPinId))
        {
            if (startPinId && endPinId)
            {
                bool startIsInput = int(startPinId.Get() - 1) % 3 == 1;
                bool endIsInput = int(endPinId.Get() - 1) % 3 == 1;
                int startNode = int(startPinId.Get() - 1) / 3;
                int endNode = int(endPinId.Get() - 1) / 3;

                if (endPinId == startPinId)
                {
                    NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                }
                else if (startIsInput == endIsInput)
                {
                    showLabel("x Incompatible Pins. Must be In->Out", ImColor(45, 32, 32, 180));
                    NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                }
                else if (startNode == endNode)
                {
                    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                    NodeEditor::RejectNewItem(ImColor(255, 0, 0), 1.0f);
                }
                else
                {
                    showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                    if (NodeEditor::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                    {
                        if (startIsInput)
                        {
                            stateMachine->AddTransition(stateMachine->nodes[endNode].name.c_str(), stateMachine->nodes[startNode].name.c_str(), "", 0);
                        }
                        else
                        {
                            stateMachine->AddTransition(stateMachine->nodes[startNode].name.c_str(), stateMachine->nodes[endNode].name.c_str(), "", 0);
                        }

                        //stateMachine->Save();
                    }
                }
            }
        }
    }
    NodeEditor::EndCreate();
}

void Hachiko::WindowStateMachine::ShowContextMenus()
{
    auto openPopupPosition = ImGui::GetMousePos();

    NodeEditor::NodeId contextNodeId = 0;
    NodeEditor::PinId contextPinId = 0;
    NodeEditor::LinkId contextLinkId = 0;

    if (NodeEditor::ShowBackgroundContextMenu())
    {
        new_node_pos = ImGui::GetMousePos();
        ImGui::OpenPopup("Add Node Menu");
    }
    else if (NodeEditor::ShowNodeContextMenu(&contextNodeId))
    {
        nodeId = int(contextNodeId.Get() - 1) / 3;
        ImGui::OpenPopup("Node Menu");
    }
    else if (NodeEditor::ShowLinkContextMenu(&contextLinkId))
    {
        linkId = int(contextLinkId.Get());
        ImGui::OpenPopup("Link Menu");
    }
}

void Hachiko::WindowStateMachine::ShowAddNodeMenu()
{
    if (ImGui::BeginPopup("Add Node Menu"))
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
                NodeEditor::SetNodePosition(stateMachine->nodes.size() * 3 + 1, NodeEditor::ScreenToCanvas(new_node_pos));
                stateMachine->AddNode(nodeName, "");
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}
/*
void Hachiko::WindowStateMachine::showDeleteLinkPopup() 
{
    if (ImNodes::IsLinkHovered(&link_id) && ImGui::IsMouseClicked(1, false))
    {
        ImGui::OpenPopup("EditLink");
    }

    if (ImGui::BeginPopup("EditLink"))
    {
        /*
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
        */
/*
        if (ImGui::Button(" Delete link "))
        {
            sm->RemoveTransitionWithTarget(sm->nodes[link_id / 100].name, sm->nodes[link_id % 100].name);
            ImGui::CloseCurrentPopup();
        }


        /*
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
        */
/*
        ImGui::EndPopup();
    }
}
*/

void Hachiko::WindowStateMachine::ShowNodeMenu() 
{
    if (ImGui::BeginPopup("Node Menu"))
    {
        ImGui::TextUnformatted("Node Menu");
        ImGui::Separator();
        
        if (ImGui::BeginMenu("Edit clip"))
        {
            if (ImGui::MenuItem("Change clip"))
            {
                stateMachine->EditNodeClip(stateMachine->nodes[nodeId].name.c_str(), "newClip");
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Change looping"))
            {
                stateMachine->EditClipLoop(stateMachine->nodes[nodeId].name.c_str(), !stateMachine->clips[stateMachine->FindClip(stateMachine->nodes[nodeId].clip.c_str())].loop);
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Delete node"))
        {
            NodeEditor::DeleteNode(NodeEditor::NodeId((nodeId + 1) * 3));
            stateMachine->RemoveNode(stateMachine->nodes[nodeId].name.c_str());
            ImGui::CloseCurrentPopup();
        }
        
        //stateMachine->Save();
        ImGui::EndPopup();
    }
}

void Hachiko::WindowStateMachine::ShowLinkMenu() 
{
    if (ImGui::BeginPopup("Link Menu"))
    {
        ImGui::TextUnformatted("Transition Menu");
        ImGui::Separator();
        /*
        char tmp[128];

        HashString trigger = animation->GetTransitionTrigger(context_link);
        snprintf(tmp, 127, trigger ? trigger.C_str() : "");
        if (ImGui::InputText("Trigger", tmp, 128))
        {
            animation->SetTransitionTrigger(context_link, HashString(tmp));
            animation->Save();
        }

        uint blend = animation->GetTransitionBlend(context_link);
        if (ImGui::DragInt("Blend", (int*)&blend, 1.0f, 0, 1000))
        {
            animation->SetTransitionBlend(context_link, blend);
            animation->Save();
        }

        ImGui::Separator();

        */
        if (ImGui::BeginMenu("Edit transition"))
        {
            if (ImGui::MenuItem("Edit trigger"))
            {
                char tmp[128];

                ImGui::InputText("Trigger", tmp, 128);
                if (tmp != "")
                {
                    stateMachine->AddTransition("Puta", "Vida", tmp, 0);
                    //stateMachine->Save();
                }
                /*
                int blend = animation->GetTransitionBlend(context_link);
                if (ImGui::DragInt("Blend", (int*)&blend, 1.0f, 0, 1000))
                {
                    animation->SetTransitionBlend(context_link, blend);
                    animation->Save();
                }
                */
            }

            if (ImGui::MenuItem("Edit interpolation time"))
            {

            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Delete"))
        {
            NodeEditor::DeleteLink(NodeEditor::LinkId((linkId + 1) + 2));
            stateMachine->RemoveTransitionWithTarget(stateMachine->nodes[linkId / 100].name, stateMachine->nodes[linkId % 100].name);
            //stateMachine->Save();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Hachiko::WindowStateMachine::OpenPopups() 
{
    if (ImGui::BeginPopup("trigger popup"))
    {
        if (ImGui::Button("B"))
        {

        }
        static char newTrigger[128] = "";
        const ImGuiInputTextFlags editTrigger_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText(" Edit trigger", newTrigger, IM_ARRAYSIZE(newTrigger), editTrigger_input_flags))
        {
            int i = 0;
            ImGui::CloseCurrentPopup();
        }
    }
}

void Hachiko::WindowStateMachine::ShowHelp() 
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
        //ImGui::BulletText("Mouse right-clicking hovering a link to edit it.");
        //ImGui::Separator();
        ImGui::Text("Node legend:");
        ImGui::Text("                  Node name");
        ImGui::Text("                Clip  | is it looping?");

        ImGui::EndPopup();
    }
}

void Hachiko::WindowStateMachine::SetStateMachine(ResourceStateMachine& resourceStateMachine) 
{
    stateMachine = &resourceStateMachine;
}


void Hachiko::WindowStateMachine::CleanUp()
{

}
