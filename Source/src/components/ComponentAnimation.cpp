#include "core/hepch.h"

#include "Application.h"

#include "modules/ModuleEditor.h"
#include "modules/ModuleResources.h"

#include "ComponentAnimation.h"
#include "ComponentTransform.h"

#include "resources/ResourceAnimation.h"
#include "animation/AnimationController.h"
#include "importers/AnimationImporter.h"

#include "ui/WindowStateMachine.h"

Hachiko::ComponentAnimation::ComponentAnimation(GameObject* container) : Component(Type::ANIMATION, container)
{
    controller = new AnimationController();
}

Hachiko::ComponentAnimation::~ComponentAnimation()
{
    delete state_machine;
    delete controller;
    animations.clear();
}

void Hachiko::ComponentAnimation::StartAnimating(unsigned int animation_index, bool on_loop, unsigned int fade_in_time_ms)
{
    if (animation_index >= animations.size())
    {
        return;
    }

    current_animation = animations[animation_index];

    StartAnimating(on_loop, fade_in_time_ms);
}

void Hachiko::ComponentAnimation::StartAnimating(bool on_loop, unsigned int fade_in_time_ms)
{
    if (!animations.empty())
    {
        controller->Play(current_animation, on_loop, fade_in_time_ms);
    }
}

void Hachiko::ComponentAnimation::StopAnimating()
{
    controller->Stop();
}

void Hachiko::ComponentAnimation::Update()
{
    if (current_animation)
    {
        controller->Update(EngineTimer::delta_time * 1000); // TODO: change for GameTimer::delta_time

        if (game_object != nullptr)
        {
            UpdatedGameObject(game_object);
        }
    }
}

void Hachiko::ComponentAnimation::UpdatedGameObject(GameObject* go)
{
    float3 position;
    Quat rotation;

    if (controller->GetTransform(controller->current, go->name.c_str(), position, rotation))
    {
        go->GetTransform()->SetLocalPosition(position);
        go->GetTransform()->SetLocalRotation(rotation);
    }

    for (GameObject* child : go->children)
    {
        UpdatedGameObject(child);
    }
}

void Hachiko::ComponentAnimation::DrawGui()
{
    ImGui::PushID(this);

    /* if (ImGuiUtils::CollapsingHeader(game_object, this, "Animation"))
    {
        constexpr int NO_DELETION = -1;
        int removed_index = NO_DELETION;
        for (unsigned i = 0; i < animations.size(); ++i)
        {
            char animation_name[50];
            strcpy_s(animation_name, 50, animations[i]->GetName().c_str());
            
            if (ImGui::Button(StringUtils::Concat(ICON_FA_PLAY, " ", animation_name, std::to_string(i)).c_str()))
            {
                current_animation = animations[i];
                this->StartAnimating(true, 200);
            }

            ImGui::SameLine();


            if (ImGui::Button(ICON_FA_STOP))
            {
                StopAnimating();
            }

            ImGuiUtils::DisplayTooltip("Pause");

            ImGui::SameLine();

            ImGui::PushID(StringUtils::Concat(std::to_string(i), "##animation").c_str());
            if (ImGui::Button(ICON_FA_TRASH))
            {
                removed_index = i;   
            }

            ImGuiUtils::DisplayTooltip("Delete");

            ImGui::PopID();
        }

        if (removed_index != NO_DELETION)
        {
            animations.erase(animations.begin() + removed_index);
        }
        
        // Animator selector
        const std::string title = StringUtils::Concat("Select Animation#", std::to_string(uid));

        if (ImGui::Button("Select animation"))
        {
            ImGuiFileDialog::Instance()->OpenDialog(title.c_str(),
                                                    "Select Animation",
                                                    ".fbx",
                                                    "./assets/models/",
                                                    1,
                                                    nullptr,
                                                    ImGuiFileDialogFlags_DisableCreateDirectoryButton | ImGuiFileDialogFlags_HideColumnType
                                                        | ImGuiFileDialogFlags_HideColumnDate);
        }

        if (ImGuiFileDialog::Instance()->Display(title.c_str()))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                
                std::string meta_path = StringUtils::Concat(ImGuiFileDialog::Instance()->GetCurrentFileName(), META_EXTENSION);
                YAML::Node meta = YAML::LoadFile("./assets/models/" + meta_path);

                for (unsigned i = 0; i < meta[RESOURCES].size(); ++i)
                {
                    Resource::Type type = static_cast<Resource::Type>(meta[RESOURCES][i][RESOURCE_TYPE].as<int>());
                    if (type == Resource::Type::ANIMATION)
                    {
                        UID res_uid = meta[RESOURCES][i][RESOURCE_ID].as<UID>();
                        ResourceAnimation* res = static_cast<ResourceAnimation*>(App->resources->GetResource(Resource::Type::ANIMATION, res_uid));
                        if (res != nullptr)
                        {
                            animations.push_back(res);
                        }
                    }
                }

            }

            ImGuiFileDialog::Instance()->Close();
        }

        // State machine displayer

        if (animations.size() > 0)
        {
            stateMachineWindow = new WindowStateMachine(game_object->name);
        }
    }*/

    if (ImGuiUtils::CollapsingHeader(game_object, this, "Animation"))
    {
        /* LOAD STATE MACHINE */

        const std::string title = StringUtils::Concat("State Machine Selector#", std::to_string(uid));
        if (ImGui::Button("Select State machine"))
        {
            ImGuiFileDialog::Instance()->OpenDialog(title.c_str(),
                                                    "Select State Machine",
                                                    ".yml",
                                                    "./library/state_machine/",
                                                    1,
                                                    nullptr,
                                                    ImGuiFileDialogFlags_DisableCreateDirectoryButton | ImGuiFileDialogFlags_HideColumnType | ImGuiFileDialogFlags_HideColumnDate);
        }
        
        if (ImGuiFileDialog::Instance()->Display(title.c_str()))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                /* std::string meta_path = StringUtils::Concat(ImGuiFileDialog::Instance()->GetCurrentFileName(), META_EXTENSION);
                YAML::Node meta = YAML::LoadFile("./assets/models/" + meta_path);

                for (unsigned i = 0; i < meta[RESOURCES].size(); ++i)
                {
                    Resource::Type type = static_cast<Resource::Type>(meta[RESOURCES][i][RESOURCE_TYPE].as<int>());
                    if (type == Resource::Type::ANIMATION)
                    {
                        UID res_uid = meta[RESOURCES][i][RESOURCE_ID].as<UID>();
                        ResourceAnimation* res = static_cast<ResourceAnimation*>(App->resources->GetResource(Resource::Type::ANIMATION, res_uid));
                        if (res != nullptr)
                        {
                            animations.push_back(res);
                        }
                    }
                }*/

                state_machine = new ResourceStateMachine(UUID::GenerateUID());
            }

            ImGuiFileDialog::Instance()->Close();
        }

        /* CREATE NEW STATE MACHINE */

        ImGui::SameLine();
        std::string auxiliary_name;

        if (ImGui::Button("New State machine"))
        {
            ImGui::OpenPopup("NewStateMachine");
        }

        if (ImGui::BeginPopup("NewStateMachine"))
        {
            auxiliary_name = game_object->name;
            ImGui::InputText("Name", &auxiliary_name, ImGuiInputTextFlags_EnterReturnsTrue);
            if (ImGui::Button("Create State Machine"))
            {
                // TODO ADD A resource creation for state machine.
                // ResourceStateMachine* res = static_cast<ResourceStateMachine*>(App->resources->CreateAsset(Resource::Type::STATE_MACHINE, auxiliary_name));

                state_machine = new ResourceStateMachine(UUID::GenerateUID());
                state_machine->state_m_name = auxiliary_name;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (state_machine != nullptr)
        {

            if (state_machine->clips.size() > 0)
            {
                windowStateMachine = new WindowStateMachine(game_object->name);
            }

            char name[128];
            strcpy_s(name, state_machine->state_m_name.c_str());

            if (ImGui::InputText("Resource name", name, 128))
            {
                state_machine->state_m_name = name;
                // state_machine->Save();
            }

            if (ImGui::Button("Add clip"))
            {
                state_machine->AddClip("noname", 0, true);
                // state_machine->Save();
            }

            ImGui::Separator();

            unsigned int i = 0;
            while (i < state_machine->clips.size())
            {
                ResourceAnimation* res = static_cast<ResourceAnimation*>(App->resources->GetResource(Resource::Type::ANIMATION, state_machine->GetClipRes(i)));

                strcpy_s(name, state_machine->clips[i].name.c_str());

                ImGui::PushID(i);
                if (ImGui::InputText("Clip name", name, 128))
                {
                    state_machine->SetClipName(i, name);
                    // state_machine->Save();
                }

                ImGui::LabelText("Resource", res ? res->GetName().c_str() : "Unknown");
                ImGui::SameLine();
                
                // CLIP selector

                const std::string title = StringUtils::Concat("Select Animation#", std::to_string(uid));
                if (ImGui::Button("Select animation"))
                {
                    ImGuiFileDialog::Instance()->OpenDialog(title.c_str(),
                                                            "Select Animation",
                                                            ".fbx",
                                                            "./assets/models/",
                                                            1,
                                                            nullptr,
                                                            ImGuiFileDialogFlags_DisableCreateDirectoryButton | ImGuiFileDialogFlags_HideColumnType | ImGuiFileDialogFlags_HideColumnDate);
                }

                if (ImGuiFileDialog::Instance()->Display(title.c_str()))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        std::string meta_path = StringUtils::Concat(ImGuiFileDialog::Instance()->GetCurrentFileName(), META_EXTENSION);
                        YAML::Node meta = YAML::LoadFile("./assets/models/" + meta_path);

                        for (unsigned i = 0; i < meta[RESOURCES].size(); ++i)
                        {
                            Resource::Type type = static_cast<Resource::Type>(meta[RESOURCES][i][RESOURCE_TYPE].as<int>());
                            if (type == Resource::Type::ANIMATION)
                            {
                                UID res_uid = meta[RESOURCES][i][RESOURCE_ID].as<UID>();
                                ResourceAnimation* res = static_cast<ResourceAnimation*>(App->resources->GetResource(Resource::Type::ANIMATION, res_uid));
                                if (res != nullptr)
                                {
                                    state_machine->AddClip("None", res->GetID(), true);
                                }
                            }
                        }
                    }
                }

                /*if (new_res > 0)
                {
                    state_machine->SetClipRes(i, new_res);
                    state_machine->Save();

                    App->resources->Get(new_res)->LoadToMemory();
                }

                bool loop = state_machine->GetClipLoop(i);
                if (ImGui::Checkbox("Loop", &loop))
                {
                    state_machine->SetClipLoop(i, loop);
                    state_machine->Save();
                }

                ImGui::SameLine();
                if (ImGui::Button("Remove"))
                {
                    state_machine->RemoveClip(i);
                    state_machine->Save();
                }
                else
                {
                    ++i;
                }

                ImGui::Separator();
                ImGui::PopID();
            }

            if (App->GetState() != Application::stop)
            {
                if (ImGui::CollapsingHeader("Animation triggers", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    HashString active_node = component->GetActiveNode();

                    for (uint i = 0; i < state_machine->GetNumTransitions(); ++i)
                    {
                        if (state_machine->GetTransitionSource(i) == active_node)
                        {
                            HashString trigger = state_machine->GetTransitionTrigger(i);
                            if (trigger && ImGui::Button(trigger.C_str()))
                            {
                                component->SendTrigger(trigger);
                            }
                        }
                    }

                    if (ImGui::Button("Reset state"))
                    {
                        component->ResetState();
                    }
                }*/
            }
        }
    }

    ImGui::PopID();
}

void Hachiko::ComponentAnimation::Save(YAML::Node& node) const
{
    for (unsigned i = 0; i < animations.size(); ++i)
    {
        node[ANIMATIONS][i] = animations[i]->GetID();
    }
}

void Hachiko::ComponentAnimation::Load(const YAML::Node& node)
{
    if (node[ANIMATIONS].IsDefined())
    {
        for (unsigned i = 0; i < node[ANIMATIONS].size(); ++i)
        {
            ResourceAnimation* r_animation = static_cast<ResourceAnimation*>(App->resources->GetResource(Resource::Type::ANIMATION, node[ANIMATIONS][i].as<UID>()));
            animations.push_back(r_animation);
        }
    }
}