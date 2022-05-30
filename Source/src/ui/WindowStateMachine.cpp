#include "core/hepch.h"
#include "WindowStateMachine.h"

#include "imnodes.h"
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

    ShowAddNodePopup();
    //ShowContextMenus();

    //NodeEditor::Suspend();
    if (NodeEditor::ShowBackgroundContextMenu())
    {
        addNode = true;
    }
    if (addNode)
    {
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiWindow* win = ImGui::GetCurrentWindow();
        ImGui::SetWindowPos(ImVec2(win->Pos.x + 100, win->Pos.y + 100), 0);
        ImGui::SetCurrentViewport(ImGui::GetCurrentWindow(), ImGui::GetCurrentWindow()->Viewport);
        NodeEditor::NodeId contextNodeId = 0;
        context_node = int(contextNodeId.Get() - 1) / 3;
        ImGui::OpenPopup("Context Menu");

         if (ImGui::BeginPopup("Context Menu"))
        {
            ImGui::TextUnformatted("Context Menu");
            ImGui::Separator();

            if (ImGui::Button(" Add node "))
            {
                addNode = false;
            }

            ImGui::Separator();

            ImGui::EndPopup();
        }
    }
    //NodeEditor::Resume();


    ShowNodeMenu();
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
        //HE_LOG("size: %f, %f", size.x, size.y);
        ImVec2 pos = NodeEditor::GetNodePosition(id + 1);
        //HE_LOG("pos: %f, %f", pos.x, pos.y);

        ImGui::Indent(1.0);
        if (stateMachine->nodes[i].name.size() <= 7)
        {
            ImGui::Text("");
            float offset = (8 - stateMachine->nodes[i].name.size()) * 0.05;
            ImGui::SameLine(size.x * offset);
        }
        ImGui::TextColored(ImVec4(255, 255, 0, 255), stateMachine->nodes[i].name.c_str());


        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f * ImGui::GetStyle().Alpha);
        ImGui::PopStyleVar();
        
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
    //NodeEditor::PushStyleVar(NodeEditor::StyleVar_LinkStrength, 4.0f);
    for (const Hachiko::ResourceStateMachine::Transition& transition : stateMachine->transitions)
    {
        int sourceID = stateMachine->FindNode(transition.source);
        int targetID = stateMachine->FindNode(transition.target);
        int linkID = sourceID * 100 + targetID;
        NodeEditor::Link(linkID, sourceID * 3 + 3, targetID * 3 + 2);
    }
    //NodeEditor::PopStyleVar(1);
}


void Hachiko::WindowStateMachine::ShowAddNodePopup()
{
    /*
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
                sm->AddNode(nodeName, "");
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    */
    /*
    if (ImGui::IsMouseDoubleClicked(0))
    {
        *id = NodeEditor::GetDoubleClickedNode();
    }


    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1,false) && ImGui::GetHoveredID() > 0)
    {
        addNode = true;       
        if (NodeEditor::ShowNodeContextMenu(id))
        {
            int j = 0;
        }
        HE_LOG("%f, %f", NodeEditor::GetNodePosition(*id).x, NodeEditor::GetNodePosition(*id).y);
    }
    
    if (addNode)
    {
        if (NodeEditor::ShowNodeContextMenu(id))
        {
            ImGui::Text("<sgsdg");
            if (ImGui::Button(" Add node "))
            {
                
            }
        }
        HE_LOG("%f, %f", NodeEditor::GetNodePosition(*id).x, NodeEditor::GetNodePosition(*id).y);
    }
    
    //ImGuiPopupFlags flags = ImGuiMouseButton_Right | ImGuiPopupFlags_NoOpenOverItems;
    //ImGui::OpenPopup("AddNode", flags);

    if (ImGui::BeginPopup("AddNode"))
    {
        HE_LOG("%f, %f", ImGui::GetMousePosOnOpeningCurrentPopup().x, ImGui::GetMousePosOnOpeningCurrentPopup().y);
        if (ImGui::Button(" Add node "))
        {
            addNode = true;
        }

        //if (addNode)
        ImGui::EndPopup();
    }

    if (addNode)
    {
        static char nodeName[128] = "";
        const ImGuiInputTextFlags nodeName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText(" Node name ", nodeName, IM_ARRAYSIZE(nodeName), nodeName_input_flags))
        {
            addNode = false;
            stateMachine->AddNode(nodeName, "");
            ImGui::CloseCurrentPopup();
        }
    }*/
}
/*
void Hachiko::WindowStateMachine::showEditNodePopup() 
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
        //else if (ImGui::Button(" Delete clip "))
        //{
            /*
            for (int i = 0; i < sm->nodes.size(); ++i)
            {
                if (sm->nodes[i].nodeIndex == node_id)
                {
                    // sm.EditStateClip(sm.nodes[i].name, "");
                }
            }
            */
            //ImGui::CloseCurrentPopup();
        //}
        //else if (ImGui::Button(" Delete node "))
        //{
            /*
            for (int i = 0; i < sm->nodes.size(); ++i)
            {
                if (sm->nodes[i].nodeIndex == node_id)
                {
                    // sm.RemoveState(nodes[i].name);
                }
            }
            */
/*/
            sm->RemoveNode(sm->nodes[node_id / 3].name);
            ImGui::CloseCurrentPopup();
        }

        if (add_clip)
        {
            static char clipName[128] = "";
            const ImGuiInputTextFlags clipName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Clip name ", clipName, IM_ARRAYSIZE(clipName), clipName_input_flags))
            {
                add_clip = false;
                for (int i = 0; i < sm->nodes.size(); ++i)
                {
                    if (sm->nodes[i].nodeIndex == node_id)
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

void Hachiko::WindowStateMachine::addLink() 
{
    int start, end;
    if (ImNodes::IsLinkStarted(&start))
    {
        started = true;
    }

    if (started && ImNodes::IsLinkCreated(&start, &end, false))
    {
        sm->AddTransition(sm->nodes[(start - 2) / 3].name, sm->nodes[(end - 1) / 3].name, "", 0);
    }
}

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

void Hachiko::WindowStateMachine::ShowContextMenus() 
{
    auto openPopupPosition = ImGui::GetMousePos();
    NodeEditor::Suspend();

    NodeEditor::NodeId contextNodeId = 0;
    NodeEditor::PinId contextPinId = 0;
    NodeEditor::LinkId contextLinkId = 0;
    //NodeEditor::GetSelectedNodes(&contextNodeId, 1);
    //contextNodeId = NodeEditor::GetDoubleClickedNode();
    //context_node = int(contextNodeId.Get() - 1) / 3;
    //HE_LOG("%i", context_node);
    if (NodeEditor::ShowNodeContextMenu(&contextNodeId))
    {
        context_node = int(contextNodeId.Get() - 1) / 3;
        ImGui::OpenPopup("Node Context Menu");
    }
    else if (NodeEditor::ShowPinContextMenu(&contextPinId))
    {
        ImGui::OpenPopup("Pin Context Menu");
    }
    else if (NodeEditor::ShowLinkContextMenu(&contextLinkId))
    {
        //context_link = int(contextLinkId.Get()) - animation->GetNumNodes() * 3 - 1;
        ImGui::OpenPopup("Link Context Menu");
    }
    else if (NodeEditor::ShowBackgroundContextMenu())
    {
        new_node_pos = ImGui::GetMousePos();
        new_node_pin = 0;
        ImGui::OpenPopup("Create New Node");
    }

    NodeEditor::Resume();
}

void Hachiko::WindowStateMachine::ShowNodeMenu() 
{
    if (ImGui::BeginPopup("Node Context Menu"))
    {
        ImGui::TextUnformatted("Node Context Menu");
        ImGui::Separator();

        char tmp[128];

        //snprintf(tmp, 127, animation->GetNodeName(context_node).C_str());
        snprintf(tmp, 127, stateMachine->nodes[context_node].name.c_str());
        if (ImGui::InputText("Name", tmp, 128))
        {
            //animation->SetNodeName(context_node, HashString(tmp));
            //animation->Save();
            stateMachine->AddNode(tmp, "");
        }
        /*
        int clip_index = animation->FindClip(animation->GetNodeClip(context_node));
        if (ImGui::Combo("Clip", &clip_index, &StateViewport::GetClip, animation, animation->GetNumClips()))
        {
            animation->SetNodeClip(context_node, animation->GetClipName(clip_index));
            animation->Save();
        }

        bool def = animation->GetDefaultNode() == context_node;
        if (ImGui::Checkbox("Default", &def))
        {
            if (def)
            {
                animation->SetDefaultNode(context_node);
            }
            else
            {
                animation->SetDefaultNode(0);
            }
        }
        */
        ImGui::Separator();

        if (ImGui::MenuItem("Delete"))
        {
            NodeEditor::DeleteNode(NodeEditor::NodeId((context_node + 1) * 3));
            //animation->RemoveNode(context_node);
            //animation->Save();
            stateMachine->RemoveNode(stateMachine->nodes[context_node].name.c_str());
        }

        ImGui::EndPopup();
    }
}

void Hachiko::WindowStateMachine::ShowLinkMenu() 
{
    if (ImGui::BeginPopup("Link Context Menu"))
    {
        ImGui::TextUnformatted("Transition Context Menu");
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

        if (ImGui::MenuItem("Delete"))
        {
            ed::DeleteLink(ed::LinkId((context_link + 1) + 2));
            animation->RemoveTransition(context_link);
            animation->Save();
        }
        */
        ImGui::EndPopup();
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
    ImNodes::DestroyContext();
}
