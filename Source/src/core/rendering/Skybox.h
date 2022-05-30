#pragma once

#include "modules/ModuleTexture.h"

namespace Hachiko
{
    class ComponentCamera;

    class Skybox
    {
    public:
        Skybox();
        Skybox(TextureCube new_cube);
        ~Skybox();
        void Draw(ComponentCamera* camera) const;

        void ChangeCubeMapSide(UID texture_uid, TextureCube::Side cube_side);
        void ReleaseCubemap();

        const TextureCube& GetCube() const
        {
            return cube;
        }

        void DrawImGui();

    private:
        void SelectSkyboxTexture(TextureCube::Side cube_side);

        void CreateBuffers();
        TextureCube cube;
        unsigned vao{};
        unsigned vbo{};

    public:
        Skybox(const Skybox& other) = default;
        Skybox& operator=(const Skybox& other) = default;
    };
}
