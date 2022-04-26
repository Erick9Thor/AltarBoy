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

        [[nodiscard]] bool HasNormal() const
        {
            return normal != nullptr;
        }

        [[nodiscard]] bool HasMetalness() const
        {
            return metalness != nullptr;
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

        [[nodiscard]] unsigned GetNomalId() const
        {
            if (normal != nullptr)
            {
                return normal->GetId();
            }
            return 0;
        }

        [[nodiscard]] unsigned GetMetalnessId() const
        {
            if (metalness != nullptr)
            {
                return metalness->GetId();
            }
            return 0;
        }

        [[nodiscard]] std::string GetName() const
        {
            return name;
        }

        void SetName(const std::string& new_name) 
        {
            name = new_name;
        }

        void SetTexture(ResourceTexture* res, ResourceTexture::Type type)
        {
            switch (type)
            {
            case ResourceTexture::Type::DIFFUSE:
                diffuse = res;
                break;            
            case ResourceTexture::Type::SPECULAR:
                specular = res;
                break;            
            case ResourceTexture::Type::NORMALS:
                normal = res;
                break;
            case ResourceTexture::Type::METALNESS:
                metalness = res;
                break;
            }
        }

        std::string TypeToString(ResourceTexture::Type type)
        {
            switch (type)
            {
            case ResourceTexture::Type::DIFFUSE:
                return "Diffuse";
            case ResourceTexture::Type::SPECULAR:
                return "Specular";
            case ResourceTexture::Type::NORMALS:
                return "Normals";
            case ResourceTexture::Type::METALNESS:
                return "Metalness";
            }
        }

        ResourceTexture* diffuse = nullptr;
        ResourceTexture* specular = nullptr;
        ResourceTexture* normal = nullptr;
        ResourceTexture* metalness = nullptr;
        float4 diffuse_color = float4::one;
        float4 specular_color = float4::one / 10.0f;
        float shininess = 50.0f;
        float smoothness = 50.0f;
        float metalness_value = 50.0f;
        bool is_metallic = false;

    private:
        void AddTexture(ResourceTexture::Type type);
        void RemoveTexture(ResourceTexture::Type type);
        void UpdateMaterial();
        std::string name;
    };
}
