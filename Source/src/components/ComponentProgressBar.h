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
            TOP_TO_BOTTOM
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
        
        float GetFilled() 
        {
            return filled;
        }
        
        FillingDirection GetDirection() 
        {
            return direction;
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
            filled = new_filled_value;
        }
        
        void SetDirection(FillingDirection new_direction)
        {
            direction = new_direction;
        }

    private:
        ComponentTransform2D* transform2D = nullptr;

        GameObject* background = nullptr;
        GameObject* foreground = nullptr;

        UID backgroundID = 0;
        UID foregroundID = 0;

        ComponentTransform2D* background_transform2D = nullptr;
        ComponentTransform2D* foreground_transform2D = nullptr;

        float min = 0.0f;
        float max = 1.0f;
        float filled = 0.0f;

        FillingDirection direction = FillingDirection::LEFT_TO_RIGHT;
        int direction_index = 0;
        inline static const char* filling_directions[] {"Left to right", "Right to Left", "Bottom to top", "Top to bottom"};
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentProgressBar::GetType()
{
    return Type::PROGRESS_BAR;
}
