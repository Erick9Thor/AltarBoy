#pragma once

#include <string>
#include <vector>

namespace Hachiko
{
    struct PathNode
    {
        PathNode() = default;

        PathNode(const PathNode& a) = default;

        std::string path;
        std::string localPath;

        std::vector<PathNode> children;

        bool isLeaf = true;
        bool isFile = true;

        void GetFilesRecursively(std::vector<PathNode*>& files)
        {
            for (PathNode& child : children)
            {
                if (child.isFile)
                {
                    files.push_back(&child);
                    continue;
                }

                child.GetFilesRecursively(files);
            }
        }

        [[nodiscard]] bool IsLastFolder() const
        {
            for (const auto& i : children)
            {
                if (i.isFile == false)
                {
                    return false;
                }
            }
            return true;
        }

        bool operator==(const PathNode node) const
        {
            return path == node.path;
        }

        PathNode& operator=(const PathNode& a)
        {
            path = a.path;
            localPath = a.localPath;

            children = std::vector(a.children);

            isLeaf = a.isLeaf;
            isFile = a.isFile;

            return *this;
        }
    };
}