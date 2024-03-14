#include <string>
#include <vector>
#include <algorithm>
#include "lib_str.h"

// remove selected character from the string
// https://www.geeksforgeeks.org/remove-spaces-from-a-given-string/
void lib_str::remove_char(std::string& str, const char& char_to_remove)
{
    // number of characters left in the string
    unsigned short count = 0;

    // if the current character is not space, then place it at index 'count++'
    for (unsigned short i = 0; str[i]; i++)
        if (str[i] != char_to_remove)
            str[count++] = str[i];
    str[count] = '\0';
    str = str.substr(0, count);
}


void lib_str::remove_char(std::vector<std::string>& str, const char& char_to_remove)
{
    for (unsigned short i = 0; i < str.size(); i++)
        remove_char(str[i], char_to_remove);
}

// split string using a separator
std::vector<std::string> lib_str::split_string(const std::string& string_to_split, const char& sep)
{
    // create a copy of the string to split
    std::string string_aux = string_to_split;

    // column separator position
    size_t pos;

    // final vector
    std::vector<std::string> splits;

    // split the string
    while (string_aux.find(sep) != std::string::npos)
    {
        // get position of the column separator
        pos = string_aux.find(sep);

        // cut row into columns
        splits.push_back(string_aux.substr(0, pos));

        // remove the split from row
        string_aux = string_aux.substr(pos + 1, string_aux.size() - pos - 1);
    }
    splits.push_back(string_aux);

    // return splits
    return splits;
}

// convert unsigned short to string with leading zeros
 std::string lib_str::to_string(const unsigned short& number, const unsigned short& length)
 {   
    std::string num_str = std::to_string(number);
    if (num_str.length() >= length)
        return num_str;
    
    std::string leading_zeros(length - num_str.length(), '0');
    return leading_zeros + num_str;
}

// convert string to an upper case
std::string lib_str::to_upper(const std::string& input_str)
{
    // convert data type to upper case
    std::string _input_str = input_str;
    std::transform(_input_str.begin(), _input_str.end(), _input_str.begin(), ::toupper);
    
    // return upper-cased string
    return _input_str;
}

// convert string to an lower case
std::string lib_str::to_lower(const std::string& input_str)
{
    // convert data type to upper case
    std::string _input_str = input_str;
    std::transform(_input_str.begin(), _input_str.end(), _input_str.begin(), ::tolower);
    
    // return upper-cased string
    return _input_str;
}

// repeat character
std::string lib_str::repeate_char(const std::string& input_str, const unsigned short& repeat_no)
{
	std::string output_str = "";
	for (unsigned short idx = 0; idx < repeat_no; idx++)
		output_str += input_str;
	return output_str;
}

// add leading character
std::string lib_str::add_leading_char(const std::string& input_str, const std::size_t& str_len, const char& add_char)
{
	if (input_str.size() < str_len)
        return std::string(str_len - input_str.size(), add_char) + input_str;
    else
        return input_str;
}

// add trailing character
std::string lib_str::add_trailing_char(const std::string& input_str, const std::size_t& str_len, const char& add_char)
{
	if (input_str.size() < str_len)
        return input_str + std::string(str_len - input_str.size(), add_char);
    else
        return input_str;
}

// reverse string
std::string lib_str::reverse_str(const std::string& input_str)
{
	std::string reversed_str = "";
	for (int char_idx = input_str.size() - 1; char_idx >= 0; char_idx--)
		reversed_str += input_str[char_idx];
	return reversed_str;
}

// auxiliary function to format number
std::string lib_str::format_num(std::string& num_str, const bool& ths_sep, const std::size_t& dec_places)
{
	// find decimal point to split whole and decimal part of the number
	std::size_t pos = num_str.find('.');

	// take care of the decimal part
	std::string decimal_part;
	if (pos == std::string::npos)
		if (dec_places == 0)
			decimal_part = "";
		else
			decimal_part = std::string(dec_places, '0');
	else
	{
		decimal_part = num_str.substr(pos + 1, num_str.length() - pos);
		if (dec_places == 0)
			decimal_part = "";
		else
			decimal_part = add_trailing_char(decimal_part, dec_places, '0');
			decimal_part = decimal_part.substr(0, dec_places);
	}

	// extract the whole part
	std::string whole_part;
	if (pos == std::string::npos)
		whole_part = num_str;
	else
		whole_part = num_str.substr(0, pos);

	// add thousand separator if needed
	std::string whole_part_ths_sep = "";
	if (ths_sep)
	{
		std::size_t counter = 0;
		whole_part = lib_str::reverse_str(whole_part);
		for (std::size_t char_idx = 0; char_idx < whole_part.size(); char_idx++)
		{
			whole_part_ths_sep += whole_part[char_idx];
			counter++;
			if ((counter == 3) & (char_idx < whole_part.size() - 1))
			{
				counter = 0;
				whole_part_ths_sep += ',';
			}
		}
		whole_part_ths_sep = reverse_str(whole_part_ths_sep);
	}

	// return the final format number
	if (ths_sep)
		if (dec_places == 0)
			return whole_part_ths_sep;
		else
			return whole_part_ths_sep + '.' + decimal_part;
	else
		if (dec_places == 0)
			return whole_part;
		else
			return whole_part + '.' + decimal_part;
}