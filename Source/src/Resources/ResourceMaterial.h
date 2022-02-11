#pragma once
#include "Resource.h"

#include "Modules/ModuleTexture.h"
#include "MathGeoLib.h"

namespace Hachiko
{
    class ResourceMaterial : public Resource
    {
    public:
        ResourceMaterial(UID uid);
        ~ResourceMaterial() override;

        [[nodiscard]] unsigned GetDiffuseId() const
        {
            return diffuse.id;
        }

        [[nodiscard]] unsigned GetSpecularId() const
        {
            return specular.id;
        }

        Texture diffuse;
        Texture specular;
        float4 diffuse_color = float4::one;
        float4 specular_color = float4::one / 10.0f;
        float shininess = 50.0f;
    };
}
