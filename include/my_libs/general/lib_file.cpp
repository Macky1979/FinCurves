#include <string>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include "lib_aux.h"
#include "lib_file.h"

// check file or path existence
bool lib_file::path_exists (const std::string& path_nm)
{
    std::filesystem::path path_nm_aux(path_nm);
    return std::filesystem::exists(path_nm_aux);
}

// delete file or path
bool lib_file::delete_path (const std::string& path_nm)
{
    std::filesystem::path path_nm_aux(path_nm);
    return std::filesystem::remove(path_nm_aux);
}

// delete files or paths
unsigned short lib_file::delete_paths (const std::vector<std::string>& path_nms)
{
    unsigned short deleted_paths_no = 0;
    bool path_deleted = false;
    for (unsigned short idx = 0; idx < path_nms.size(); idx++)
        path_deleted = lib_file::delete_path (path_nms[idx]);
        if (path_deleted)
            deleted_paths_no++;
    return deleted_paths_no;
}

// delete all files in path
unsigned short lib_file::delete_all_files (const std::string& path_nm)
{
    std::filesystem::path path_nm_aux(path_nm);
    return std::filesystem::remove_all(path_nm_aux);
}

// get vector of files in a folder
std::vector<std::string> lib_file::get_file_list(const std::string& path_nm)
{
    std::vector<std::string> file_nms;
    for (const auto& entry: std::filesystem::directory_iterator(path_nm))
        file_nms.push_back(entry.path());
    return file_nms;
}

// create folder
bool lib_file::create_folder(const std::string& path_nm)
{
    return std::filesystem::create_directory(path_nm);
}
