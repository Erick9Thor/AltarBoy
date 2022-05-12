#pragma once

#include "core/preferences/src/ResourcesPreferences.h"

namespace Hachiko
{
    class Resource;

    class Importer
    {
    public:
        enum class Type
        {
            SCENE = 0,
            MODEL,
            MESH,
            TEXTURE,
            MATERIAL,
            ANIMATION,
            FONT,
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


        virtual void Import(const char* path, YAML::Node& meta) = 0;
        virtual void Save(const Resource* resource) = 0;
        virtual Resource* Load(UID id) = 0;
        virtual void Delete(UID uid, Resource::Type resource_type) 
        {
            std::string library_path = StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(resource_type), std::to_string(uid));
            FileSystem::Delete(library_path.c_str());
        };
    protected:
        Type type;
        [[nodiscard]] ResourcesPreferences* GetResourcesPreferences()
        {
            if (preferences == nullptr)
            {
                preferences = App->preferences->GetResourcesPreference();
            }

            return preferences;
        }

     private:
        ResourcesPreferences* preferences = nullptr;
    };
}
