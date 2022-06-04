#pragma once

#include "Component.h"

namespace Hachiko
{
    class GameObject;
    class AnimationController;
    class ResourceAnimation;
    class ResourceStateMachine;

    class WindowStateMachine;
  
    class ComponentAnimation : public Component
    {
    public:
        ComponentAnimation(GameObject* container);
        ~ComponentAnimation() override;

        HACHIKO_API void StartAnimating();
        HACHIKO_API void StopAnimating();

        void ResetState();
        void SendTrigger(const std::string& trigger);

        void Update() override;

        void DrawGui() override;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

    private:
        void LoadStateMachine();
        void AnimationSelector(unsigned clip_idx);

        void UpdatedGameObject(GameObject* go);
        void PlayNode(const std::string& node, unsigned int blend);
        void PlayNode(unsigned int node_idx, unsigned int blend);
        std::string GetActiveNode() const;

    private:
        AnimationController* controller = nullptr;
        ResourceStateMachine* state_machine = nullptr;

        WindowStateMachine* windowStateMachine = nullptr;

        unsigned int active_node = 0;

        unsigned editing_clip_idx = 0;
        YAML::Node selected_model_meta;
        std::vector<std::string> selected_asset_clip_names;
    };
} // namespace Hachiko
