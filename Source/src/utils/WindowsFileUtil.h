#pragma once

#include <string>

namespace Hachiko
{
bool GetFileLastWriteTimestamp(const std::wstring& file_name, 
	std::string& timestamp);

FILETIME GetFileLastWriteTime(const std::wstring& file_name);
} // namespace Hachiko