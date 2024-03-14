#include <iostream>
#include <string>
#include <vector>
#include "lib_settings.h"

int main()
{
    // read settings file
    std::string file_nm = "/home/macky/Documents/Programming/C++/FinCurves/include/my_libs/general/data/lib_settings/csc_corp#mig_mtrx.settings";
    std::vector<std::vector<std::string>> parameters = lib_settings::load_settings(file_nm, ':');

    // print settings
    for (unsigned short idx = 0; idx < parameters.size(); idx++)
        std::cout << parameters[idx][0] << ": " << parameters[idx][1] << std::endl;
 
    // everything OK
    return 0;
}