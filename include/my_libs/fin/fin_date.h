/** \example fin_date_examples.h
 * @file fin_date.h
 * @author Michal Mackanic
 * @brief Functions for date manipulations.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <memory>
#include "lib_date.h"

namespace fin_date
{
    /**
     * @brief Decompose frequency string into frequency amount and unit, e.g. 1M into [1, "M"].
     * 
     * @param freq_str Frequency string, e.g. "1M".
     * @return std::tuple<unsigned short, std::string> Frequency amount and unit.
     */
    std::tuple<int, std::string> decompose_freq(const std::string& freq_str);

    /**
     * @brief Compose frequency string from frequency amount and frequency unit, e.g. [1, "M"] into "1M".
     * 
     * @param freq_decomposed Frequency amount and frequency string.
     * @return std::string Frequency string.
     */
    std::string compose_freq(const std::tuple<int, std::string>& freq_decomposed);

    /**
     * @brief Convert frequency amount and frequency unit, e.g. [1, "M"], into proxy maturit, e.g. 1 / 12 = 0.083333.
     * 
     * @param freq_decomposed Frequency amount and frequency unit, e.g. [1, "M"].
     * @return double Proxy maturity, e.g. 1 / 12 = 0.083333.
     */
    double get_tenor_maturity(const std::tuple<int, std::string>& freq_decomposed);

    /**
     * @brief Determine year fraction between two dates using a specified day count method. For more details see https://en.wikipedia.org/wiki/Day_count_convention.
     * 
     * @param date_begin Begining date.
     * @param date_end End date.
     * @param dcm Day count method to be applied; supported values are "30_360", "30US_360", "ACT_360", "ACT_365" and "ACT_ACT".
     * @return double Year fraction.
     */
    double day_count_method(const lib_date::myDate& date_begin, const lib_date::myDate& date_end, const std::string& dcm);

    /**
     * @brief Create a list of frequency amounts and units, e.g. [[1, "M"], [2, "M"], [3, "M"], ...], till maturity specified in years.
     * 
     * @param freq_decomposed Frequency amount and frequency unit, e.g. [1, "M"].
     * @param maturity Maturity for which the list of frequency amounts and units is to be projected.
     * @return std::vector<std::tuple<int, std::string>> List of projected frequency amounts and units.
     */
    std::vector<std::tuple<int, std::string>> generate_freqs(const std::tuple<int, std::string>& freq_decomposed, const unsigned short& maturity = 30);

    /**
     * @brief Drop duplicates from a list of frequency amounts and units, e.g. in a form of [[1, "M"], [2, "M"], [3, "M"], ...].
     * 
     * @param freqs_decomposed List of frequency amounts and units, e.g. [[1, "M"], [2, "M"], [3, "M"], ...] to be checked for duplicities.
     * @return std::vector<std::tuple<int, std::string>> List of frequency amounts and frequency units, e.g. [[1, "M"], [2, "M"], [3, "M"], ...] without duplicities.
     */
    std::vector<std::tuple<int, std::string>> drop_freq_duplicates(const std::vector<std::tuple<int, std::string>>& freqs_decomposed);

    /**
     * @brief Sort list of frequency amounts and units in a form of [[1, "M"], [2, "M"], [3, "M"], ...].
     * 
     * @param freqs_decomposed List of frequency amounts and frequency units, e.g. [[1, "M"], [2, "M"], [3, "M"], ...] to be sorted.
     * @return std::vector<std::tuple<int, std::string>> Sorted list of frequency amounts and frequency units, e.g. [[1, "M"], [2, "M"], [3, "M"], ...]
     */
    std::vector<std::tuple<int, std::string>> sort_freqs(const std::vector<std::tuple<int, std::string>>& freqs_decomposed);

    /**
     * @brief Add or subtract two decomposed frequencies provided they have the same frequecy unit, e.g. [1, "M"] and [2, "M"].
     * 
     * @param freq1 The first frequency amount and unit.
     * @param freq2 The second frequency amount and unit.
     * @param operand "+" => Add the two decomposed frequencies. '\n' "-" => Subtract the two decomposed frequencies.
     * @return std::tuple<int, std::string> Result frequency amount and unit.
     */
    std::tuple<int, std::string> combine_freqs(const std::tuple<int, std::string>& freq1, const std::tuple<int, std::string>& freq2, const std::string& operand = "+");

    /**
     * @brief Determine year fractions between dates organized in a vector using a specified day count method. For more details see https://en.wikipedia.org/wiki/Day_count_convention.
     * 
     * @param dates Vector of dates.
     * @param dcm Day count method to be applied; supported values are "30_360", "30US_360", "ACT_360", "ACT_365" and "ACT_ACT".
     * @return std::vector<double> Vector of year fractions; the vector is one element shorter compared to vector of dates.
     */
    std::vector<double> day_count_method(const std::vector<lib_date::myDate>& dates, const std::string& dcm);

    /**
     * @brief Determine year fraction between dates in the vector and reference date using a specified day count method. For more details see https://en.wikipedia.org/wiki/Day_count_convention.
     * 
     * @param date_ref Reference date.
     * @param dates Vector of dates.
     * @param dcm Day count method to be applied; supported values are "30_360", "30US_360", "ACT_360", "ACT_365" and "ACT_ACT".
     * @return std::vector<double> Vector of year fractions; the vector is of the same length as vector of dates.
     */
    std::vector<double> day_count_method(const lib_date::myDate& date_ref, const std::vector<lib_date::myDate>& dates, const std::string& dcm);

    /**
     * @brief Determine year fraction between dates in two vectors using a specified day count method. The two vectors must be of the same length. For more details see https://en.wikipedia.org/wiki/Day_count_convention.
     * 
     * @param dates_begin Vector of dates defining beging of the period.
     * @param dates_end  Vector of dates defining end of the period.
     * @param dcm Day count method to be applied; supported values are "30_360", "30US_360", "ACT_360", "ACT_365" and "ACT_ACT".
     * @return std::vector<double> Vector of year fractions; the vector is of the same length as the two vector of dates.
     */
    std::vector<double> day_count_method(const std::vector<lib_date::myDate>& dates_begin, const std::vector<lib_date::myDate>& dates_end, const std::string& dcm);

    /**
     * @brief Roll the date to deal with weekends and public holidays if necessary. For more details see https://en.wikipedia.org/wiki/Date_rolling.
     * 
     * @param date Date to be rolled for weekend / public holidays if necessary.
     * @param holidays List of public holidays.
     * @param drm Date rolling method; suppoted values are "following", "modified_following", "previous" and "modified_previous".
     */
    void date_rolling(lib_date::myDate& date, const std::vector<lib_date::myDate>& holidays, const std::string& drm);

    /**
     * @brief Roll the dates to deal with weekends and public holidays if necessary. For more details see https://en.wikipedia.org/wiki/Date_rolling.
     * 
     * @param dates Vector of dates to be rolled for weekend / public holidays if necessary.
     * @param holidays List of public holidays.
     * @param drm Date rolling method; suppoted values are "following", "modified_following", "previous" and "modified_previous".
     */
    void date_rolling(std::vector<lib_date::myDate>& dates, const std::vector<lib_date::myDate>& holidays, const std::string& drm);

    /**
     * @brief Create a date serie based on a begining date and vector of frequency strings. During the serie construction country and day rolling method are specifed so that the resut consists of working days only.
     * 
     * @param date_start Start date in string format.
     * @param date_freqs Vector of frequency  strings.
     * @param cnty Countery name determining public holidays calendar.
     * @param drm Day rolling method.
     * @param date_format Date format, e.g. "yyyymmdd".
     * @return std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>> Tuple of date vectors. The first vector represents period begining and the second represents period end. For example, in case of "ON", the first date corresponds to the begining date and the second date is the next working day.
     */
    std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>> create_date_serie(const std::string& date_start_str, const std::vector<std::string>& date_freqs, const std::string& cnty, const std::string& drm, const std::string& date_format = "yyyymmdd");
}
