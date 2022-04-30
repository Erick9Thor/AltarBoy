#pragma once

#include "core/hepch.h"
#include "utils/WindowsFileUtil.h"

#include <Windows.h>
#include <strsafe.h>

bool Hachiko::GetFileLastWriteTimestamp(const std::wstring& file_name, 
	std::string& timestamp) 
{
	auto file_time_last_write = GetFileLastWriteTime(file_name);
    if (!file_time_last_write.dwHighDateTime && !file_time_last_write.dwLowDateTime)
	{
		return false;
	}

	char timestamp_buffer[MAX_PATH];
    SYSTEMTIME utc_time, local_time;
    DWORD timestamp_dword;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&file_time_last_write, &utc_time);
	SystemTimeToTzSpecificLocalTime(NULL, &utc_time, &local_time);

	// Build a string showing the date and time.
	timestamp_dword = StringCchPrintf(timestamp_buffer,
		MAX_PATH,
		"%02d/%02d/%d %02d:%02d:%02d ms%d",
		local_time.wDay,
		local_time.wMonth,
		local_time.wYear,
		local_time.wHour,
		local_time.wMinute,
		local_time.wSecond,
		local_time.wMilliseconds);
	timestamp = timestamp_buffer;

	if (timestamp_dword == S_OK)
	{
		return true;
	}

	return false;
}

FILETIME Hachiko::GetFileLastWriteTime(const std::wstring& file_name)
{
    HANDLE file_handle = CreateFileW(file_name.data(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        CloseHandle(file_handle);
        return FILETIME();
    }

    FILETIME file_time_create;
    FILETIME file_time_last_access;
    FILETIME file_time_last_write;

    // Retrieve the file times for the file.
    bool file_time_retriaval_success = GetFileTime(file_handle, &file_time_create, &file_time_last_access, &file_time_last_write);
    CloseHandle(file_handle);
	
	return file_time_last_write;
}