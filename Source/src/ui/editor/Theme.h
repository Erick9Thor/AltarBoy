#pragma once
#include <string>

namespace Hachiko::Editor
{
    class Theme final
    {
    public:
        enum class Type
        {
            DARK = 0,
            LIGHT,
            PINK
        };

        static const char* ToString(Type theme)
        {
            switch (theme)
            {
            case Type::LIGHT:
                return "light";
            case Type::PINK:
                return "pink";
            case Type::DARK: default:
                return "dark";
            }
        }

        static Type FromString(const std::string& theme)
        {
            if (theme == "light")
            {
                return Type::LIGHT;
            }
            if (theme == "pink")
            {
                return Type::PINK;
            }
            return Type::DARK;
        }

        Theme() = delete;
    };
}
