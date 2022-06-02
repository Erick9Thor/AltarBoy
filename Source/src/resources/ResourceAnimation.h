#pragma once

#include "resources/Resource.h"
#include <unordered_map>

#include <memory>
#include <MathGeoLibFwd.h>


namespace Hachiko
{
    class ResourceAnimation : public Resource
    {
    public:

        enum class State
        {
            PLAYING, // While the animation is playing
            PAUSED, // If the animation is paused
            STOPPED, // When the animation is finished
            UNSET
        };

        struct Channel
        {
            Channel() = default;
            Channel(Channel&& o) = default;
            Channel& operator=(Channel&& o) = default;

            std::unique_ptr<float3[]> positions;
            std::unique_ptr<Quat[]> rotations;
            unsigned int num_positions = 0;
            unsigned int num_rotations = 0;
        };

    public:
        ResourceAnimation(UID id);
        virtual ~ResourceAnimation();

        // RESOURCE INFO
        [[nodiscard]] std::string GetName() const
        {
            return name;
        }
        void SetName(const char* new_name)
        {
            name = new_name;
        }

        // ANIMATION CONTROLL
        unsigned int GetDuration() const
        {
            return duration;
        }

        void SetDuration(unsigned int i_duration)
        {
            duration = i_duration;
        }

        // CHANEL MANAGE
        [[nodiscard]] unsigned int GetNumChannels() const
        {
            return static_cast<unsigned>(channels.size());
        }

        [[nodiscard]] const Channel* GetChannel(const std::string& name) const;


        State GetCurrentState() const
        {
            return current_state;
        }

        void SetCurrentState(State new_current_state)
        {
            current_state = new_current_state;
        }

        std::unordered_map<std::string, Channel> channels;

    private:
        //std::vector<Node> nodes;
        unsigned int duration = 0;

        std::string name;

        State current_state = State::UNSET;
    };
} // namespace Hachiko
