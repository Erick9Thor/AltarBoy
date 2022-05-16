#pragma once

#include "core/preferences/src/ResourcesPreferences.h"

namespace Hachiko
{
    class Resource;

    class Importer
    {
    public:
        Importer() = default;
        virtual ~Importer() = default;

        virtual void Import(const char* path, YAML::Node& meta) = 0;
        virtual void Save(UID id, const Resource* resource) = 0;
        virtual Resource* Load(UID id) = 0;

        std::string GetResourcePath(Resource::Type type, UID id)
        {
            return StringUtils::Concat(GetResourcesPreferences()->GetLibraryPath(type), std::to_string(id));
        }

        virtual void Delete(UID uid, Resource::Type resource_type)
        {
                FileSystem::Delete(GetResourcePath(resource_type, uid).c_str());
        }

        bool ExistsInMetaArray(YAML::Node& node, unsigned index)
        {
            if (node.IsDefined() && node.size() > index)
            {
                return true;
            }
            return false;
        }

        // With current meta state and resource it aligns the meta and returns the corresponding uid
        // If uid existed and type remains the same maintains the same id reference, generates a new one otherwise
        UID ManageResourceUID(Resource::Type type, unsigned int resource_index, YAML::Node& meta)
        {
            // Resource Reference doesnt exist
            if (!ExistsInMetaArray(meta[RESOURCES], resource_index))
            {
                meta[RESOURCES][resource_index][RESOURCE_ID] = UUID::GenerateUID();
                meta[RESOURCES][resource_index][RESOURCE_TYPE] = static_cast<int>(type);
            }
            // Resource reference changed type
            else if (static_cast<Resource::Type>(meta[RESOURCES][resource_index][RESOURCE_TYPE].as<int>()) != type)
            {
                meta[RESOURCES][resource_index][RESOURCE_ID] = UUID::GenerateUID();
                meta[RESOURCES][resource_index][RESOURCE_TYPE] = static_cast<int>(type);
            }
            return meta[RESOURCES][resource_index][RESOURCE_ID].as<UID>();
        }
        Resource::Type GetResourceTypeFromMeta(unsigned int resource_index, YAML::Node& meta)
        {
            // Assumes the resource definition exists
            return static_cast<Resource::Type>(meta[RESOURCES][resource_index][RESOURCE_TYPE].as<int>());
        }

        void AddResource(UID uid, Resource::Type type, YAML::Node& meta)
        {
            unsigned new_index = 0;
            if (meta[RESOURCES].IsDefined() && meta[RESOURCES].IsSequence())
            {
                new_index = meta[RESOURCES].size();
            }
            meta[RESOURCES][new_index][RESOURCE_ID] = uid;
            meta[RESOURCES][new_index][RESOURCE_TYPE] = static_cast<int>(type);
        }

        void SetResource(UID uid, Resource::Type type, unsigned resource_index, YAML::Node& meta)
        {
            meta[RESOURCES][resource_index][RESOURCE_ID] = uid;
            meta[RESOURCES][resource_index][RESOURCE_TYPE] = static_cast<int>(type);
        }

        void RemoveResourcesOverIndex(unsigned resource_index, YAML::Node& meta)
        {
            if (meta[RESOURCES].IsDefined())
            {
                for (unsigned i = resource_index; i < meta[RESOURCES].size(); ++i)
                {
                    meta[RESOURCES].remove(i);
                }
            }
        }



    protected:
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
