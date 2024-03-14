/** \example lib_str_examples.h
 * @file lib_str.h
 * @author Michal Mackanic
 * @brief  String related C++ functions.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <math.h>

namespace lib_str
{
    /**
     * @brief Remove character from string.
     * 
     * @param str String from which the character is to be removed.
     * @param char_to_remove Character to be removed.
     */
    void remove_char(std::string& str, const char& char_to_remove = ' ');

    /**
     * @brief Remove character from strings in vector.
     * 
     * @param str Vector of strings from which the character is to be removed.
     * @param char_to_remove Character to be removed.
     */
    void remove_char(std::vector<std::string>& str, const char& char_to_remove = ' ');

    /**
     * @brief Split string with a character used as a separator.
     * 
     * @param string_to_split String to be split.
     * @param sep Character to be used as a separator.
     * @return std::vector<std::string> Vector consisting of the split part of the original string.
     */
    std::vector<std::string> split_string(const std::string& string_to_split, const char& sep);

    // convert unsigned short to string with leading zeros
    /**
     * @brief Convert unsigned short to a string with leading zeros.
     * 
     * @param number Number to be converted into a string with leading zeros.
     * @param length Length of the resulting string. Leading zeros are used to achieve a desired string length.
     * @return std::string String consisting of the original number and possibly leading zeros.
     */
    std::string to_string(const unsigned short& number, const unsigned short& length);

    /**
     * @brief Convert string to upper case.
     * 
     * @param input_str String to be converted to upper case.
     * @return std::string Converted string.
     */
    std::string to_upper(const std::string& input_str);

    /**
     * @brief Convert string to lower case.
     * 
     * @param input_str String to be converted to lower case.
     * @return std::string Converted string.
     */
    std::string to_lower(const std::string& input_str);

    /**
     * @brief Repeate string.
     * 
     * @param input_str String to be repeated.
     * @param repeat_no Number of repetitions.
     * @return std::string Resulting string.
     */
    std::string repeate_char(const std::string& input_str, const unsigned short& repeat_no = 1);

    /**
     * @brief Add leading character to exisitng string to achieve a desired length.
     * 
     * @param input_str String to which leading charater should be added.
     * @param str_len Desired length of the resulting string.
     * @param add_char Leading character.
     * @return std::string Resulting string.
     */
    std::string add_leading_char(const std::string& input_str, const std::size_t& str_len, const char& add_char = ' ');

    /**
     * @brief Add trailing character to exisitng string to achieve a desired length.
     * 
     * @param input_str String to which trailing charater should be added.
     * @param str_len Desired length of the resulting string.
     * @param add_char Trailing character.
     * @return std::string Resulting string.
     */
    std::string add_trailing_char(const std::string& input_str, const std::size_t& str_len, const char& add_char = ' ');

    /**
     * @brief Reverse string.
     * 
     * @param input_str Original string.
     * @return std::string Reversed string.
     */
    std::string reverse_str(const std::string& input_str);

    /**
     * @brief Auxiliary function that formats number expressed as a string.
     * 
     * @param num_str Number converted into a string.
     * @param ths_sep true => Use thousand separator. \n false => Do not user thousand separator.
     * @param dec_places Number of decimal places to display.
     * @return std::string Resulting string.
     */
    std::string format_num(std::string& num_str, const bool& ths_sep, const std::size_t& dec_places);

    /**
     * @brief Convert number into a string and format it.
     * 
     * @tparam T Numerial type of the number to be converted into string and formated.
     * @param input_num Number to be converted into string and formated.
     * @param ths_sep true => Use thousand separator. \n false => Do not user thousand separator.
     * @param dec_places Number of decimal places to display.
     * @param round true => Round the number to required number of decimal places before its convertion to string. \n Do not round the number before its conversion to string.
     * @return std::string Resulting string.
     */
    template <class T>
    std::string format_num(const T& input_num, const bool& ths_sep = true, const std::size_t& dec_places = 2, const bool& round = true)
    {
        // convert number to stringNumber of decimal places to display.
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