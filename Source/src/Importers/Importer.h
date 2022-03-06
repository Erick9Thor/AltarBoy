#pragma once

namespace Hachiko
{
    class Resource;
    class ResourcesPreferences;

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
        virtual void Save(const Resource* resource) = 0;
        virtual Resource* Load(const UID id) = 0;

    protected:
        ResourcesPreferences* preferences = nullptr;
        Type type;
    };
}
