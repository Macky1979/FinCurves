/*
#include <iostream>
#include <string>
#include <vector>
#include "lib_settings.h"

int main()
{
    // read settings file
    std::string file_nm = "data/fin_mig_mtrx/csc_corp#na#00#ttc.settings";
    std::vector<std::vector<std::string>> parameters = lib_settings::load_settings(file_nm, ':');

    // print settings
    for (unsigned short idx = 0; idx < parameters.size(); idx++)
        std::cout << parameters[idx][0] << ": " << parameters[idx][1] << std::endl;
 
    // everything OK
    return 0;
}
*/
# pragma once

#include <string>
#include <vector>
#include <iostream>

namespace lib_settings
{
    std::vector<std::vector<std::string>> load_settings(const std::string& file_nm, const char& sep = ':');
}