/** \example lib_file_examples.h
 * @file lib_file.h
 * @author Michal Mackanic
 * @brief  Files and folders C++ functions.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <vector>

namespace lib_file
{
    /**
     * @brief Check path existence.
     * 
     * @param path_nm Path to be checked.
     * @return true Path exists.
     * @return false Path does not exist.
     */
    bool path_exists (const std::string& path_nm);

    /**
     * @brief Get vector of files in a folder.
     * 
     * @param path_nm Path to the folder.
     * @return std::vector<std::string> Vector of file names in the folder.
     */
    std::vector<std::string> get_file_list(const std::string& path_nm);

    /**
     * @brief Delete file or folder defined through a path.
     * 
     * @param path_nm Path which defines file or folder.
     * @return true File or folder successfully deleted.
     * @return false Problems with file or folder deletion.
     */
    bool delete_path (const std::string& path_nm);

    /**
     * @brief Delete files or folders defined through a vector of paths.
     * 
     * @param path_nms Vector of paths defining files or folders to be deleted.
     * @return unsigned short Number of deleted files or folders.
     */
    unsigned short delete_paths (const std::vector<std::string>& path_nms);

    /**
     * @brief Delete all files in a folder defined through user specified path.
     * 
     * @param path_nm Path defining folder in which all files are to be deleted.
     * @return unsigned short Number of deleted files.
     */
    unsigned short delete_all_files (const std::string& path_nm);

    /**
     * @brief Create a folder specified through a path.
     * 
     * @param path_nm Path defining folder to be created.
     * @return true Folder successfully created.
     * @return false Folder was not created.
     */
    bool create_folder(const std::string& path_nm);
}