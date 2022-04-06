#pragma once

#include "Component.h"
#include "ComponentTransform2D.h"
#include "Globals.h"

namespace Hachiko
{
    class GameObject;

    class ComponentProgressBar : public Component
    {
    public:
        enum class FillingDirection
        {
            LEFT_TO_RIGHT,
            RIGHT_TO_LEFT,
            BOTTOM_TO_TOP,
            TOP_TO_BOTTOM,
        };

        ComponentProgressBar(GameObject* container);
        ~ComponentProgressBar() override = default;

        static Type GetType();

        void Update() override;
        void DrawGui() override;

        float GetMin() 
        {
            return min;
        }

        float GetMax() 
        {
            return max;
        }
        
        float GetFilledValue() 
        {
            return filled_value;
        }
        
        FillingDirection GetDirection() 
        {
            return fill_direction;
        }
        
        void SetMin(float new_min) 
        {
            min = new_min;
        }

        void SetMax(float new_max)
        {
            max = new_max;
        }
        
        void SetFilledValue(float new_filled_value)
        {
            filled_value = new_filled_value;
        }
        
        void SetDirection(FillingDirection new_direction)
        {
            fill_direction = new_direction;
        }

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

    private:
        GameObject* background = nullptr;
        GameObject* fill = nullptr;

        float min = 0.0f;
        float max = 1.0f;
        float filled_value = 0.0f;

        FillingDirection fill_direction = FillingDirection::LEFT_TO_RIGHT;
        int direction_index = 0;
        inline static const char* filling_directions[] {"Left to right", "Right to Left", "Bottom to top", "Top to bottom"};
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentProgressBar::GetType()
{
    return Type::PROGRESS_BAR;
}
