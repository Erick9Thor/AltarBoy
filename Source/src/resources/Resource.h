#pragma once

#include "utils/UUID.h"

namespace Hachiko
{
    class Resource
    {
    public:
        enum class Type
        {
            UNKNOWN,
            MATERIAL,
            TEXTURE,
            MODEL,
            MESH,
            SCENE,
            SHADER,
            AUDIO,
            ANIMATION,
            SCRIPT,
            VIDEO,
            SKYBOX,
            FONT,
            NAVMESH,
            COUNT
        };

        static std::string TypeString(const Type& t) {
            static const std::map<Type, std::string> string_map = {
                {Type::MATERIAL, "materials"},
                {Type::TEXTURE, "textures"},
                {Type::MODEL, "models"},
                {Type::MESH, "meshes"},
                {Type::SCENE, "scene"},
                {Type::SHADER, "shaders"},
                {Type::AUDIO, "audio"},
                {Type::ANIMATION, "animations"},
                {Type::SCRIPT, "scripts"},
                {Type::VIDEO, "video"},
                {Type::SKYBOX, "skybox"},
                {Type::FONT, "fonts"},
                {Type::NAVMESH, "navmesh"},
            };

            auto it = string_map.find(t);
            if (it != string_map.end())
            {
                return it->second;
            }
            return "";
        }

        Resource(Type type);
        Resource(UID id, Type type);
        virtual ~Resource();

        [[nodiscard]] Type GetType() const
        {
            return type;
        }

        [[nodiscard]] UID GetID() const
        {
            return id;
        }

        unsigned int instances = 0;

    private:
        UID id = 0;
        Type type = Type::UNKNOWN;
    };
}
