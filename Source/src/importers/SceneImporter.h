#pragma once

namespace Hachiko
{
    class Scene;

    namespace SceneImporter
    {
        Scene* LoadScene(const char* file_path);
        bool SaveScene(Scene* scene, const char* file_path);
    } // namespace SceneImporter
}
