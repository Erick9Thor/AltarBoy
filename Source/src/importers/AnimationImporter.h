#pragma once

#include <assimp/scene.h>

namespace Hachiko
{
    class ResourceAnimation;

    class AnimationImporter final : public Importer
    {
        friend class ModelImporter;
    public:
        AnimationImporter();
        ~AnimationImporter() override = default;

        void Import(const char* path, UID uid) override;
        void Save(const Resource* resource) override;
        Resource* Load(UID id) override;

    private:
        void ImportSingleAnimation(const aiAnimation* animation, UID id);
    };
} // namespace Hachiko
