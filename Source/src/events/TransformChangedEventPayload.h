#pragma once

#include "components/ComponentTransform.h"

namespace Hachiko
{
    struct TransformChangedEventPayload
    {
        TransformChangedEventPayload(ComponentTransform* transform_comp, GameObject* comp_owner) :
            transform(transform_comp) , owner(comp_owner) {}

        [[nodiscard]] ComponentTransform* GetTransform() const
        {
            return transform;
        }

        [[nodiscard]] GameObject* GetOwner() const
        {
            return owner;
        }

    private:
        ComponentTransform* transform;
        GameObject* owner;
    };
}