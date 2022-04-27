#pragma once

#include "Resource.h"
#include <memory>

class GLFont;

namespace Hachiko
{
       
    class ResourceFont : public Resource
    {
    public:
        ResourceFont();
        ResourceFont(UID id);
        ~ResourceFont() override = default;

        std::unique_ptr<GLFont> gl_font = nullptr;
    };
} // namespace Hachiko
