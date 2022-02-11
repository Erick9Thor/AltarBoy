#pragma once

#include <string>
#include <vector>

struct PathNode
{
	PathNode()
		: path("") {};
	PathNode(const PathNode& a)
		: path(a.path)
		, localPath(a.localPath)
		, children(a.children)
		, isLeaf(a.isLeaf)
		, isFile(a.isFile)
	{}

	std::string path;
	std::string localPath;

	std::vector<PathNode> children;

	bool isLeaf = true;
	bool isFile = true;

	bool IsLastFolder() const
	{
		for (unsigned int i = 0; i < children.size(); i++)
			if (children[i].isFile == false)
				return false;
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

		children = std::vector<PathNode>(a.children);

		isLeaf = a.isLeaf;
		isFile = a.isFile;

		return *this;
	}
};
