/** \example lib_settings_examples.h
 * @file lib_settings.h
 * @author Michal Mackanic
 * @brief  C++ functions to parse settings files.
 * @version 1.0
 * @date 2024-02-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

# pragma once

#include <string>
#include <vector>
#include <iostream>

namespace lib_settings
{
    /**
     * @brief Load settings from a text file.
     * 
     * @param file_nm Name of text file with settings.
     * @param sep Separator used to delimit parameter name and its value.
     * @return std::vector<std::vector<std::string>> Vector of vectors of strings representing parameter name and parameter value.
     */
    std::vector<std::vector<std::string>> load_settings(const std::string& file_nm, const char& sep = ':');
}