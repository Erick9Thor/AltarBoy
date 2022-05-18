#pragma once

#include "Resource.h"

namespace Hachiko
{
    class ResourceStateMachine : public Resource
    {
    public:
        struct Clip
        {
            std::string name;
            UID animation = 0;
            bool loop = false;

            Clip() {};
            Clip(const std::string& name, UID animation, bool loop) : name(name), animation(animation), loop(loop) {};
        };

        void AddClip(const std::string& name, UID animation, bool loop);
        int FindClip(const std::string& name) const;
        void RemoveClip(const std::string& name);
        void EditClipAnimation(const std::string& name, UID newAnimation);
        void EditClipLoop(const std::string& name, bool newLoop);

        struct Node
        {
            std::string name;
            std::string clip;

            int nodeIndex;
            int inputIndex;

            Node() {};
            Node(const std::string& name, const std::string& clip) : name(name), clip(clip) {};
        };

        void AddNode(const std::string& name, const std::string& clip);
        int FindNode(const std::string& name) const;
        std::vector<int> FindNodeTransitions(const std::string& name) const;
        void RemoveNode(const std::string& name);
        void EditNodeClip(const std::string& name, const std::string& newClip);

        struct Transition
        {
            std::string source;
            std::string target;
            std::string trigger;
            unsigned int interpolationTime = 0;

            Transition() {};
            Transition(const std::string& source, const std::string& target,const std::string& trigger, unsigned int interpolationTime) : source(source), target(target), trigger(trigger), interpolationTime(interpolationTime) {};
        };

        void AddTransition(const std::string& source, const std::string& target, const std::string& trigger, unsigned int interpolationTime);
        int FindTransitionWithTrigger(const std::string& source, const std::string& trigger) const;
        int FindTransitionWithTarget(const std::string& source, const std::string& target) const;
        std::vector<struct Transition> FindTransitionsFromNode(const std::string& name) const;
        void RemoveTransitionWithTrigger(const std::string& source, const std::string& trigger);
        void RemoveTransitionWithTarget(const std::string& source, const std::string& target);
        void EditTransitionSource(const std::string& newSource, const std::string& target, const std::string& trigger);
        void EditTransitionTarget(const std::string& source, const std::string& newTarget, const std::string& trigger);
        void EditTransitionTrigger(const std::string& source, const std::string& target, const std::string& newTrigger);
        void EditTransitionInterpolationTime(const std::string& source, const std::string& target, unsigned int newInterpolationTime);


    public:
        ResourceStateMachine(UID uid);
        ~ResourceStateMachine() override;

        std::vector<Clip> clips;
        std::vector<Node> nodes;
        std::vector<Transition> transitions;
    };
} // namespace Hachiko
