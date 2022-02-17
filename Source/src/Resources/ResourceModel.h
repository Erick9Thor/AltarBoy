#pragma once

#include "Resource.h"
#include "Globals.h"

namespace Hachiko
{
    class ResourceModel : public Resource
    {
    public:
        ResourceModel(UID uid);
        ~ResourceModel() override;
    };
}


