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

        UID LoadAnimation();
        void LoadStateMachine();

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

        std::vector<ResourceAnimation*> animations;

    private:
        AnimationController* controller = nullptr;
        ResourceAnimation* current_animation = nullptr;
        ResourceStateMachine* state_machine = nullptr;

        WindowStateMachine* windowStateMachine = nullptr;
    };
} // namespace Hachiko
