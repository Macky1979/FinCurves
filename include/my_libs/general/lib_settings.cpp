#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "lib_str.h"
#include "lib_settings.h"

std::vector<std::vector<std::string>> lib_settings::load_settings(const std::string &file_nm, const char &sep)
{
    // vector of vector of strings
    std::vector<std::vector<std::string>> settings;

    // parameter name and its value
    std::vector<std::string> parameter; 

    // row read from the .csv file
    std::string row;

    // open file
    std::ifstream f;
    f.open(file_nm, std::ios::out);

    // check that the file is indeed opened
    if (f.is_open())
    {
        // read the values row by row
        while (getline(f, row))
        {
            // split the row to parameter name and its value
            parameter = lib_str::split_string(row, sep);
            // remove spaces
            lib_str::remove_char(parameter, ' ');
            // store parameter and its value
            settings.push_back(parameter);
        }

        // close file
        f.close();
    }
    else
        throw std::runtime_error((std::string)__func__ + ": Unable to open file " + file_nm + "!");
    
    // return vector with parameter values
    return settings;
}