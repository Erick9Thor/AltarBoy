#pragma once
#include "Module.h"

#include "utils/PathNode.h"

#include <string>
#include <vector>

namespace Hachiko
{
    class ModuleFileSystem : public Module
    {
    public:
        ModuleFileSystem();
        ~ModuleFileSystem() override;

        bool Init() override;

        void CreateContext();

        // --- CRUD --- //
        void CreateDir(const char* directory_path) const;
        char* Load(const char* file_path) const;
        bool Save(const char* file_path, const void* buffer, unsigned int size, bool append = false) const;
        bool Save(const char* path, const YAML::Node& node) const;
        void Delete(const char* file_path) const;

        // --- Folder utils --- //
        static bool Copy(const char* source_file_path, const char* destination_file_path, bool fail_if_exist = false);
        static std::string NormalizePath(const char* full_path);
        static bool Exists(const char* file_path);
        static bool IsDirectory(const char* directory_path);

        // --- File utils --- //
        static void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr);
        std::string GetFileNameAndExtension(const char* file_path) const;
        std::string GetFileExtension(const char* file_path) const;
        static bool HasExtension(const char* path);
        static bool HasExtension(const char* path, std::vector<std::string> extensions);

        // --- Path utils --- //
        PathNode GetAllFiles(const char* directory, std::vector<std::string>* filter_ext, std::vector<std::string>* ignore_ext) const;
        static void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list);

        std::string GetWorkingDirectory()
        {
            return working_directory;
        }

    private:
        std::string working_directory;
    };
}
