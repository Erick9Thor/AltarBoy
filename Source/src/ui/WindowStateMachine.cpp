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
    if (!ImGui::Begin((std::string(ICON_FA_BEZIER_CURVE " ") + stateMachine->state_m_name.c_str() + " - Left Alt + H for help").c_str(), &active))
    {
        ImGui::End();
        return;
    }

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
    NodeEditor::Resume();
   
    ShowHelp();

    if (ImGui::IsKeyDown(SDL_SCANCODE_ESCAPE))
    {
        addNode = false;
        editTrigger = false;
        editIT = false;
    }

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
        if (ImGui::MenuItem(" Add node "))
        {
            addNode = true;
        }

        ImGui::EndPopup();
    }

    if (addNode)
    {
        ImGui::OpenPopup("addNode");
        if (ImGui::BeginPopup("addNode"))
        {
            static char nodeName[128] = "";
            snprintf(nodeName, 128, "");
            const ImGuiInputTextFlags nodeName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;

            if (ImGui::InputText(" Node name ", nodeName, IM_ARRAYSIZE(nodeName), nodeName_input_flags))
            {
                addNode = false;
                NodeEditor::SetNodePosition(stateMachine->nodes.size() * 3 + 1, NodeEditor::ScreenToCanvas(new_node_pos));
                stateMachine->AddNode(nodeName, "");
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}

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

        if (ImGui::BeginMenu("Edit transition"))
        {
            if (ImGui::MenuItem("Edit trigger"))
            {
                editTrigger = true;
            }

            if (ImGui::MenuItem("Edit interpolation time"))
            {
                editIT = true;
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

    if (editTrigger)
    {
        ImGui::OpenPopup("editTrigger");
        if (ImGui::BeginPopup("editTrigger"))
        {
            std::string sourceName = stateMachine->nodes[linkId / 100].name;
            std::string targetName = stateMachine->nodes[linkId % 100].name;
            const char* trigger = stateMachine->transitions[stateMachine->FindTransitionWithTarget(sourceName, targetName)].trigger.c_str();

            static char newTrigger[128] = "";
            snprintf(newTrigger, 128, trigger ? trigger : "");
            const ImGuiInputTextFlags editTrigger_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;

            if (ImGui::InputText(" Edit trigger", newTrigger, IM_ARRAYSIZE(newTrigger), editTrigger_input_flags))
            {
                editTrigger = false;
                stateMachine->EditTransitionTrigger(sourceName, targetName, newTrigger);
                //stateMachine->Save();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

    }

    if (editIT)
    {
        ImGui::OpenPopup("editIT");
        if (ImGui::BeginPopup("editIT"))
        {
            std::string sourceName = stateMachine->nodes[linkId / 100].name;
            std::string targetName = stateMachine->nodes[linkId % 100].name;
            unsigned int it = stateMachine->transitions[stateMachine->FindTransitionWithTarget(sourceName, targetName)].interpolationTime;

            static char newIT[128] = "";
            snprintf(newIT, 128, std::to_string(it).c_str());
            const ImGuiInputTextFlags editIT_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;

            if (ImGui::InputText(" Edit interpolation time", newIT, IM_ARRAYSIZE(newIT), editIT_input_flags))
            {
                editIT = false;
                stateMachine->EditTransitionInterpolationTime(stateMachine->nodes[linkId / 100].name, stateMachine->nodes[linkId % 100].name, std::stoi(newIT));
                //stateMachine->Save();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
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
        ImGui::BulletText("Mouse left-click while moving the mouse to move around the editor.");
        ImGui::Separator();
        ImGui::BulletText("Mouse left-clicking in the background to add a node.");
        ImGui::Separator();
        ImGui::BulletText("Mouse left-clicking hovering a node to open Node Menu.");
        ImGui::Separator();
        ImGui::BulletText("Mouse right-clicking hovering a pin to start a link");
        ImGui::Text("   and release it in another pin to create it.");
        ImGui::Separator();
        ImGui::BulletText("Mouse left-clicking hovering a link to open Transition Menu.");
        ImGui::Separator();
        ImGui::BulletText("ESC to close a menu.");
        ImGui::Separator();
        ImGui::Text("Node legend:           ______________________");
        //ImGui::Text("                      |       Node name      |");
        ImGui::Text("                      |      ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(255, 255, 0, 255),"Node name");
        ImGui::SameLine();
        ImGui::Text("     |");
        ImGui::Text("                      | Clip name | looping? |");
        ImGui::Text("                      | In               Out |");

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
