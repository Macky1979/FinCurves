/*
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

int main ()
{
    // try to open and parse sample YAML file
    try
    {
        // read the sample YAML file
        YAML::Node config = YAML::LoadFile("../data/lib_yaml/sample.yaml");

        // the outer element is an array
        for(auto dict : config)
        {
            // the array element is a map containing the triangle and coordinates keys:
            auto name = dict["triangle"];
            auto coordinates = dict["coordinates"];

            // print triangle name
            std::cout << "triangle: " << name << '\n';

            // loop over the triangle coordinates and print them
            for (auto coordinate : coordinates)
                std::cout << "   " << coordinate["x"] << ", " << coordinate["y"] << std::endl;
        }
    }
    // catch problems with the file
    catch (const YAML::BadFile& e)
    {
        std::cerr << e.msg << std::endl;
        return 1;
    }
    // catch problem with the parsing
    catch (const YAML::ParserException& e)
    {
        std::cerr << e.msg << std::endl;
        return 1;
    }

    // everything is OK
    return 0; 
}
*/