#pragma once
#include "yaml-cpp/yaml.h"
#include <Math/float2.h>
#include <Math/float3.h>
#include <Math/float4.h>
#include <Math/Quat.h>
#include <Math/float4x4.h>

#include "assimp/vector3.h"

namespace YAML
{
    template<>
    struct convert<float2>
    {
        static Node encode(const float2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, float2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<float3>
    {
        static Node encode(const float3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, float3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<float4>
    {
        static Node encode(const float4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, float4& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Quat>
    {
        static Node encode(const Quat& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Quat& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<float4x4>
    {
        static Node encode(const float4x4& rhs)
        {
            Node node;

            node.push_back(rhs.scaleX);
            node.push_back(rhs.shearXy);
            node.push_back(rhs.shearXz);
            node.push_back(rhs.x);

            node.push_back(rhs.shearYx);
            node.push_back(rhs.scaleY);
            node.push_back(rhs.shearYz);
            node.push_back(rhs.y);

            node.push_back(rhs.shearZx);
            node.push_back(rhs.shearZy);
            node.push_back(rhs.scaleZ);
            node.push_back(rhs.z);

            node.push_back(rhs.shearWx);
            node.push_back(rhs.shearWy);
            node.push_back(rhs.shearWz);
            node.push_back(rhs.w);

            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, float4x4& rhs)
        {
            if (!node.IsSequence() || node.size() != 16)
            {
                return false;
            }

            rhs.scaleX = node[0].as<float>();
            rhs.shearXy = node[1].as<float>();
            rhs.shearXz = node[2].as<float>();
            rhs.x = node[3].as<float>();

            rhs.shearYx = node[4].as<float>();
            rhs.scaleY = node[5].as<float>();
            rhs.shearYz = node[6].as<float>();
            rhs.y = node[7].as<float>();

            rhs.shearZx = node[8].as<float>();
            rhs.shearZy = node[9].as<float>();
            rhs.scaleZ = node[10].as<float>();
            rhs.z = node[11].as<float>();

            rhs.shearWx = node[12].as<float>();
            rhs.shearWy = node[13].as<float>();
            rhs.shearWz = node[14].as<float>();
            rhs.w = node[15].as<float>();

            return true;
        }
    };

    template<>
    struct convert<aiVector3D>
    {
        static Node encode(const aiVector3D& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, aiVector3D& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
} // namespace YAML
