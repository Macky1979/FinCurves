/*
#include <iostream>
#include <string>
#include <vector>
#include "lib_str.h"

int main()
{
    // remove character from a string
    {
        std::cout << "REMOVE CHARACTER FROM A STRING" << std::endl;
        std::string ahoj = "A  h o   j";
        std::cout << ahoj << std::endl;
        lib_str::remove_char(ahoj, ' ');
        std::cout << ahoj << std::endl;
        std::cout << '\n' << std::endl;
    }
    // remove character from a vector of strings
    {
        std::cout << "REMOVE CHARACTER FROM A VECTOR OF STRINGS" << std::endl;
        std::vector<std::string> ahoj = {"A  h o   j", " Na z da   r"};
        for (int i = 0; i < ahoj.size(); i++)
            std::cout << ahoj[i] << std::endl;
        lib_str::remove_char(ahoj, ' ');
         for (int i = 0; i < ahoj.size(); i++)
            std::cout << ahoj[i] << std::endl;       
        std::cout << '\n' << std::endl;
    }

    // split string
    {
        std::cout << "SPLIT STRING" << std::endl;
        std::string string_to_split = "a,h,o,j";
        std::vector<std::string> splits = lib_str::split_string(string_to_split, ',');
        for (int i = 0; i < splits.size(); i++)
            std::cout << splits[i] << std::endl;
        std::cout << '\n' << std::endl;
    }

    // leading zeros
    {
        std::cout << "LEADING ZEROS" << std::endl;
        unsigned short number = 10;

        std::cout << lib_str::to_string(number, 2) << std::endl;
        std::cout << lib_str::to_string(number, 3) << std::endl;

        std::cout << '\n' << std::endl;
    }

    // cast to lower and upper case
    {
        std::cout << "CAST TO UPPER AND LOWER CASE" << std::endl;

        std::string x = "hello!";
        std::string y = "BYE!";

        std::cout << x << " -> " << lib_str::to_upper(x) << std::endl;
        std::cout << y << " -> " << lib_str::to_lower(y) << std::endl;

        std::cout << '\n' << std::endl;
    }
 
    // repeat character
    {
        std::cout << "REPEATING CHARACTER" << std::endl;

        std::string input_str = "^-";
        unsigned short repeat_no = 5;
        std::string output_str = lib_str::repeat_char(input_str, repeat_no);
        std::cout << "input string:  " + input_str << std::endl;
        std::cout << "output string: " + output_str << std::endl;

        std::cout << '\n' << std::endl;
    }

    // leading and trailing character
    {
        std::cout << "LEADING AND TRAILING CHARACTER" << std::endl;

        std::string number = "123";
        std::size_t str_len = 6;
        char add_char = '0';

        std::cout << number << " -> " << lib_str::add_leading_char(number, str_len, add_char) << std::endl;
        std::cout << number << " -> " << lib_str::add_trailing_char(number, str_len, add_char) << std::endl;

        std::cout << '\n' << std::endl;
    }

    // reverse string
    {
        std::cout << "REVERSE STRING" << std::endl;
        std::string input_str = "Ahoj!";
        std::string reversed_str = lib_str::reverse_str(input_str);
        std::cout << input_str << " -> " << reversed_str << std::endl;
        std::cout << '\n' << std::endl;
    }

    // format number
    {
        std::cout << "FORMAT NUMBER" << std::endl;
        double num = 123545.598;
        bool ths_sep = true;
        std::size_t dec_places = 2;
        bool round = true;
        std::string formated_num = lib_str::format_num(num, ths_sep, dec_places, round);

        std::cout << std::to_string(num) << " -> " << formated_num << std::endl;
        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}
*/

#pragma once

#include <string>
#include <vector>
#include <math.h>

namespace lib_str
{
    // remove selected character from the string
    void remove_char(std::string& str, const char& char_to_remove = ' ');
    void remove_char(std::vector<std::string>& str, const char& char_to_remove = ' ');

    // split string
    std::vector<std::string> split_string(const std::string& string_to_split, const char& sep);

    // convert unsigned short to string with leading zeros
    std::string to_string(const unsigned short& number, const unsigned short& length);

    // convert string to an upper case
    std::string to_upper(const std::string& input_str);

    // convert string to an upper case
    std::string to_lower(const std::string& input_str);

    // repeat character
    std::string repeat_char(const std::string& input_str, const unsigned short& repeat_no = 1);

    // add leading character
    std::string add_leading_char(const std::string& input_str, const std::size_t& str_len, const char& add_char = ' ');

    // add trailing character
    std::string add_trailing_char(const std::string& input_str, const std::size_t& str_len, const char& add_char = ' ');

    // reverse string
    std::string reverse_str(const std::string& input_str);

    // auxiliary function to format the number
    std::string format_num(std::string& num_str, const bool& ths_sep, const std::size_t& dec_places);

    // format number
    template <class T>
    std::string format_num(const T& input_num, const bool& ths_sep = true, const std::size_t& dec_places = 2, const bool& round = true)
    {
        // convert number to string
        std::string num;
        if (round)
        {
            T aux = std::pow(10, dec_places);
            T input_num_aux = (T)std::round(input_num * aux) / aux;
            num = std::to_string(input_num_aux);
        }
        else
            num = std::to_string(input_num);
        
        // format the number
        return format_num(num, ths_sep, dec_places);
    };
}