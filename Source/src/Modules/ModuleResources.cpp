#include "core/hepch.h"

#include "ModuleResources.h"
#include "Application.h"
#include "ModuleEvent.h"
#include "ModuleFileSystem.h"

#include "Core/preferences/src/ResourcesPreferences.h"

using namespace Hachiko;

bool ModuleResources::Init()
{
    preferences = static_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));

    // create assets & library directory tree
    for (int i = 0; i < static_cast<int>(Resource::Type::UNKNOWN); ++i)
    {
        App->file_sys->CreateDir(preferences->GetLibraryPath(static_cast<Resource::Type>(i)));
        App->file_sys->CreateDir(preferences->GetAssetsPath(static_cast<Resource::Type>(i)));
    }

    std::function handleAddedFile = [&](Event& evt)
    {
        const auto& e = evt.GetEventData<FileAddedEventPayload>();
        std::filesystem::path file = e.GetPath();
        HE_LOG("Handling dropped file: %s", file.string().c_str());
        HandleResource(file);
    };
    App->event->Subscribe(Event::Type::FILE_ADDED, handleAddedFile);

    // TODO: iterate over the 

    // load meshes
    // load textures
    // load materials
    // load models

    return true;
}

bool ModuleResources::CleanUp()
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
        delete it->second;

    for (auto it = textures.begin(); it != textures.end(); ++it)
        delete it->second;

    for (auto it = materials.begin(); it != materials.end(); ++it)
        delete it->second;

    for (auto it = models.begin(); it != models.end(); ++it)
        delete it->second;

    return Module::CleanUp();
}

std::filesystem::path ModuleResources::GetLastResourceLoadedPath() const
{
    return last_resource_path;
}

void ModuleResources::HandleResource(const std::filesystem::path& path)
{
    Resource::Type type = GetType(path);
    if (type == Resource::Type::UNKNOWN)
    {
        HE_LOG("Unknown resource type recevied, nothing to be done");
        return;
    }
    std::filesystem::path destination = preferences->GetAssetsPath(type);

    if(App->file_sys->Copy(path.string().c_str(),
        destination.append(path.filename().c_str()).string().c_str(), true))
    {
        last_resource_path = path; // We may need this to import more assets from this path
        HandleAssetsChanged(destination, type);
        HE_LOG("File destination: %s", destination.string().c_str());
    }
}

Resource::Type ModuleResources::GetType(const std::filesystem::path& path)
{
    if (!path.has_extension())
    {
        return Resource::Type::UNKNOWN;
    }
    const std::filesystem::path extension = path.extension();

    auto isValidExtension = [&](const std::pair<Resource::Type, std::string>& element)
    {
        return element.second.size() == extension.string().size() &&
            std::equal(element.second.begin(), element.second.end(), extension.string().begin(),
                       [](auto a, auto b) { return std::tolower(a) == std::tolower(b); });
    };

    const auto it = std::find_if(supported_extensions.begin(), supported_extensions.end(), isValidExtension);
    if (it != supported_extensions.end())
    {
        return it->first;
    }
    return Resource::Type::UNKNOWN;
}

void ModuleResources::HandleAssetsChanged(const std::filesystem::path& asset_path, const Resource::Type asset_type)
{
    Event assetChanged(Event::Type::ASSETS_CHANGED);
    assetChanged.SetEventData<AssetsAddedEventPayload>(asset_path, asset_type);
    App->event->Publish(assetChanged);
}
