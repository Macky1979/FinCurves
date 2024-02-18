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
     * @brief Determine year fraction between two dates using a specified day count method. For more details see https://en.wikipedia.org/wiki/Day_count_convention.
     * 
     * @param date_begin Begining date.
     * @param date_end End date.
     * @param dcm Day count method to be applied; supported values are "30_360", "30US_360", "ACT_360", "ACT_365" and "ACT_ACT".
     * @return double Year fraction.
     */
    double day_count_method(const lib_date::myDate& date_begin, const lib_date::myDate& date_end, const std::string& dcm);

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
