#pragma once

#include "utils/UUID.h"

namespace Hachiko
{
    class Importer
    {
    public:
        enum class Type
        {
            SCENE = 0,
            MODEL,
            MESH,
            TEXTURE,
            COUNT
        };

        Importer(const Type type) : type(type)
        {
        }

        virtual ~Importer() = default;

        [[nodiscard]] Type GetType() const
        {
            return type;
        }

        virtual void Import(const char* path) = 0;
        virtual void Save() = 0;
        virtual void Load(UID id) = 0;

    protected:
        ResourcesPreferences* preferences = nullptr;
        Type type;
    };
}
