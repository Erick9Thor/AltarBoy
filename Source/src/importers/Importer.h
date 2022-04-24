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
        virtual void ImportWithMeta(const char* path, YAML::Node& meta) = 0;
        virtual void Save(const Resource* resource) = 0;
        virtual Resource* Load(UID id) = 0;
        virtual void Delete(const YAML::Node& meta) 
        {
            UID meta_uid = meta[GENERAL_NODE][GENERAL_ID].as<UID>();
            Resource::Type asset_type = static_cast<Resource::Type>(meta[GENERAL_NODE][GENERAL_TYPE].as<int>());
            std::string library_path = StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(asset_type), std::to_string(meta_uid));
            
            FileSystem::Delete(library_path.c_str());
        };
        virtual bool IsImported(const char* path) = 0;

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
