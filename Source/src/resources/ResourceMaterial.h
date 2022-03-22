#pragma once
#include "Resource.h"

#include "resources/ResourceTexture.h"

namespace Hachiko
{

    class ResourceMaterial : public Resource
    {
    public:
        ResourceMaterial(UID uid);
        ~ResourceMaterial() override;

        void DrawGui();

        [[nodiscard]] bool HasDiffuse() const
        {
            return diffuse != nullptr;
        }

        [[nodiscard]] bool HasSpecular() const
        {
            return specular != nullptr;
        }

        [[nodiscard]] bool HasNormals() const
        {
            return normals != nullptr;
        }

        [[nodiscard]] unsigned GetDiffuseId() const
        {
            if (diffuse != nullptr)
            {
                return diffuse->GetId();
            }
            return 0;
        }

        [[nodiscard]] unsigned GetSpecularId() const
        {
            if (specular != nullptr)
            {
                return specular->GetId();
            }
            return 0;
        }

        [[nodiscard]] unsigned GetNomalsId() const
        {
            if (normals != nullptr)
            {
                return normals->GetId();
            }
            return 0;
        }

        std::string name;
        ResourceTexture* diffuse = nullptr;
        ResourceTexture* specular = nullptr;
        ResourceTexture* normals = nullptr;
        float4 diffuse_color = float4::one;
        float4 specular_color = float4::one / 10.0f;
        float shininess = 50.0f;
    };
}
