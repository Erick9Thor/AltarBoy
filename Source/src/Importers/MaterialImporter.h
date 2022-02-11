#pragma once

#include "Utils/JsonFormatterValue.h"
#include "Utils/UUID.h"

#include "assimp/scene.h"

namespace Hachiko
{
    class ResourceMaterial;

    namespace MaterialImporter::Material
    {
        ResourceMaterial* Import(const aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name);
        bool Save(const ResourceMaterial* material, JsonFormatterValue j_material);
        ResourceMaterial* Load(JsonFormatterValue j_material);
    } // namespace MaterialImporter::Material
}
