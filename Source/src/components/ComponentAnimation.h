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

        HACHIKO_API void StartAnimating(unsigned int animation_index, bool on_loop = true, unsigned int fade_in_time_ms = 0);
        HACHIKO_API void StartAnimating(bool on_loop = true, unsigned int fade_in_time_ms = 0);
        HACHIKO_API void StopAnimating();
        
        void Update() override;

        void UpdatedGameObject(GameObject* go);

        void DrawGui() override;

        void AnimationSelector(unsigned clip_idx);
        void LoadStateMachine();

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

        ResourceAnimation* GetCurrentAnimation()
        {
            return current_animation;
        }

        std::vector<ResourceAnimation*> animations;

    private:
        AnimationController* controller = nullptr;
        ResourceAnimation* current_animation = nullptr;
        ResourceStateMachine* state_machine = nullptr;

        WindowStateMachine* windowStateMachine = nullptr;

        unsigned editing_clip_idx = 0;
        YAML::Node selected_model_meta;
        std::vector<std::string> selected_asset_clip_names;
    };
} // namespace Hachiko
