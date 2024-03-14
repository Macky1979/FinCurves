/** \example lib_date_examples.h
 * @file lib_date.h
 * @author Michal Mackanic
 * @brief  Date related C++ functions.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace lib_date
{
    /**
     * @brief Date class.
     * 
     */
    class myDate
    {
        private:
            // variables
            std::string date_str;
            std::size_t date_int;
            std::size_t year;
            std::size_t month;
            std::size_t day;
            std::size_t days_no;
            unsigned short day_in_week;
            const std::size_t init_year = 1601;

            // functions
            void set_year_month_day();
            void set_days_no();
            void set_day_in_week();

        public:
            /**
             * @brief Construct a new myDate object and initialize it to reference date 01/01/1601.
             * 
             */
            myDate();

            /**
             * @brief Construct a new myDate object and initialize it to user specified date.
             * 
             * @param date_str Date in string format.
             * @param date_format Date string format, e.g. "yyyyddmm".
             */
            myDate(const std::string& date_str, const std::string& date_format);

            /**
             * @brief Construct a new myDate object and initialize it to user specified date.
             * 
             * @param date_int Date in integer format, e.g. 19791230.
             */
            myDate(const std::size_t& date_int);

            /**
             * @brief Copy constructor.
             * 
             * @param date 
             */
            myDate (const myDate& date);

            /**
             * @brief Destroy the myDate object.
             * 
             */
            ~myDate(){};

            /**
             * @brief Overloaded "minus" operator "-".
             * 
             * @param date2 Date to right of the operator that should be be subtracted from the date to the left of the operator.
             * @return long Difference between two dates in calendar days.
             */
            long operator- (const myDate& date2);

            /**
             * @brief Overloaded "assign" operator "=".
             * 
             * @param _date Date to right of the operator to be assigned to date on left of the operator.
             * @return myDate& Date to which value was assigned.
             */
            myDate& operator= (const myDate& _date);

            /**
             * @brief Overloaded "equal" operator "==".
             * 
             * @param _date Date to the right of the operator that should be compared to the date to the left of the operator.
             * @return true The two dates are equal.
             * @return false The two dates are not equal.
             */
            bool operator== (const myDate& _date);

            /**
             * @brief Overloaded "smaller than" operater "<".
             * 
             * @param date Date to the right of the operator that should be compared to the date to the left of the operator.
             * @return true The first date is smaller.
             * @return false The first date is greater.
             */
            bool operator< (const myDate& date);

            /**
             * @brief Overloaded "greater than" operater ">".
             * 
             * @param date Date to the right of the operator that should be compared to the date to the left of the operator.
             * @return true The first date is greater.
             * @return false The first date is smaller.
             */
            bool operator> (const myDate& date);

            /**
             * @brief Overloaded "smaller than or equal" operator "<=".
             * 
             * @param date Date to the right of the operator that should be compared to the date to the left of the operator.
             * @return true The first date is smaller or equal.
             * @return false The first date is greater.
             */
            bool operator<= (const myDate& date);

            /**
             * @brief Overloaded "greater than or equal" operator "<=".
             * 
             * @param date Date to the right of the operator that should be compared to the date to the left of the operator.
             * @return true The first date is greater or equal.
             * @return false The first date is smaller.
             */
            bool operator>= (const myDate& date);

            /**
             * @brief Recalculate myDate object (typically after adjusting the object manually).
             * 
             */
            void recalc();

            /**
             * @brief Set date in string format.
             * 
             * @param date_str Date in string format.
             * @param date_format Date string format, e.g. "yyyyddmm".
             */
            void set(const std::string& date_str, const std::string& date_format);

            /**
             * @brief Set date in integer format.
             * 
             * @param date_int Date in integer format, e.g. 19791230.
             */
            void set(const std::size_t& date_int);

            /**
             * @brief Get year.
             * 
             * @return std::size_t Year.
             */
            std::size_t get_year() const {return year;}

            /**
             * @brief Get month in year.
             * 
             * @return std::size_t Month.
             */
            std::size_t get_month() const {return month;}

            /**
             * @brief Get day in month.
             * 
             * @return std::size_t Day.
             */
            std::size_t get_day() const {return day;}

            /**
             * @brief Get number of days from a reference date of 01/01/1601.
             * 
             * @return std::size_t Number of days from the reference date.
             */
            std::size_t get_days_no() const {return days_no;}

            /**
             * @brief Get the day in week.
             * 
             * @return unsigned short Day in week (Monday = 1, Sunday = 7).
             */
            unsigned short get_day_in_week() const {return day_in_week;}

            /**
             * @brief Get the date in integer format.
             * 
             * @return std::size_t Date in integer format.
             */
            std::size_t get_date_int() const {return date_int;}

            /**
             * @brief Get the date in string format of "yyyymmdd".
             * 
             * @return std::string Date in string format.
             */
            std::string get_date_str() const {return date_str;}

            /**
             * @brief Add period expressed in form of frequency string to the date.
             * 
             * @param date_freq Frequency string, e.g. "1D", "2M", "5Y", etc.
             */
            void add(const std::string& date_freq);

            /**
             * @brief Subtract period expressed in form of frequency string from the date.
             * 
             * @param date_freq Frequency string, e.g. "1D", "2M", "5Y", etc.
             */
            void remove(const std::string& date_freq);

            /**
             * @brief Check if the date is the last day in month.
             * 
             * @return true Date is the last day in month.
             * @return false Date is not the last day in month.
             */
            bool is_last_day_in_month() const;

            /**
             * @brief Check if the date belongs to a leap year.
             * 
             * @return true Date belongs to a leap year.
             * @return false Date does not belong to a leap year.
             */
            bool is_leap_year() const;

            /**
             * @brief Check if the date belongs to a weekend.
             * 
             * @return true Date belongs to a weekend.
             * @return false Date does not belong to a weekend.
             */
            bool is_weekend() const;

            /**
             * @brief Check if the date belongs to public holidays.
             * 
             * @param holidays Vector of dates representing public holidays.
             * @return true Date belongs to public holidays.
             * @return false Date does not belong to public holidays.
             */
            bool is_holidays(const std::vector<myDate>& holidays) const;

            /**
             * @brief Check if the date is a working date.
             * 
             * @param holidays Vector of dates representing public holidays.
             * @return true Date is a working day.
             * @return false Date is not a working day.
             */
            bool is_working_day(const std::vector<myDate>& holidays) const;

            /**
             * @brief Add working days to the date.
             * 
             * @param holidays Vector of dates representing public holidays.
             * @param working_days_no Number of working days to be added.
             */
            void add_working_days(const std::vector<myDate>& holidays, const std::size_t& working_days_no = 1);

            /**
             * @brief Set date to next working day after the current date.
             * 
             * @param holidays Vector of dates representing public holidays.
             */
            void set_to_next_working_day(const std::vector<myDate>& holidays);

            /**
             * @brief Set date to previous working day before the current date.
             * 
             * @param holidays Vector of dates representing public holidays.
             */
            void set_to_prev_working_day(const std::vector<myDate>& holidays);
    };

    /**
     * @brief Get numbr of days in month.
     * 
     * @param year Year (relevent for February where number days depend on if the year is a leap year or not).
     * @param month Month.
     * @return std::size_t Number of days in month.
     */
    std::size_t days_in_month(const std::size_t& year, const std::size_t& month);

    /**
     * @brief Add months to a date specified through year, month and day.
     * 
     * @param year Year.
     * @param month Month.
     * @param day Day.
     * @param months_to_add Number of months to add.
     * @return std::vector<std::size_t> Vector containing year, month and day of the updated date.
     */
    std::vector<std::size_t> add_months(const std::size_t& year, const std::size_t& month, const std::size_t& day, const std::size_t& months_to_add);

    /**
     * @brief Subtract months from a date specified through year, month and day.
     * 
     * @param year Year.
     * @param month Month.
     * @param day Day.
     * @param months_to_remove Number of months to subtract.
     * @return std::vector<std::size_t> Vector containing year, month and day of the updated date.
     */
    std::vector<std::size_t> remove_months(const std::size_t& year, const std::size_t& month, const std::size_t& day, const std::size_t& months_to_remove);

    /**
     * @brief Decompose frequency string, e.g. "1M" into tuple (1, "M").
     * 
     * @param freq Frequency string, e.g. "1D", "2M", "3Y", etc.
     * @return std::tuple<std::size_t, std::string> Tuple holding number of time units and time unit type, e.g. (1, "M").
     */
    std::tuple<std::size_t, std::string> decompose_freq(const std::string& freq);

    /**
     * @brief Convert frequency string into double to approximately measure its time length in years.
     * 
     * @param freq Frequency string.
     * @return double Approximate time length of the frequency string in years.
     */
    double eval_freq(const std::string& freq);

    /**
     * @brief Create a date series.
     * 
     * @param date_str_begin Begining date in string format, e.g. "19791230".
     * @param date_str_end End date in string format, e.g. "20240222".
     * @param date_freq Frequency string which determines step length of individual periods between begining and end date.
     * @param date_format Date format of the begining and end date, e.g. "yyyymmdd".
     * @return std::vector<myDate> Vector of dates between begining and end date distance among which is determined by the frequency string.
     */
    std::vector<myDate> create_date_serie(const std::string& date_str_begin, const std::string& date_str_end, const std::string& date_freq, const std::string& date_format = "yyyymmdd");

    /**
     * @brief Get index of date from a vector of dates, which is the nearest to the user specified date.
     * 
     * @param date User specified date, which is used as a reference point.
     * @param dates Vector of dates.
     * @param type "smaller" => Select the nearest smaller date. \n "larger" => Select the nearest larger date. \n "abs" => Select the nearest date in absolute terms.
     * @return std::size_t 
     */
    std::size_t get_nearest_idx(const myDate& date, const std::vector<myDate>& dates, const std::string& type);

    /**
     * @brief Get date of Easter Sunday.
     * 
     * @param year Year for which the date of Easter Sunday should be determined.
     * @return myDate Date of Easter Sunday.
     */
    myDate get_easter_sunday(const std::size_t& year);

    /**
     * @brief Get date of Easter Monday.
     * 
     * @param year Year for which the date of Easter Monday should be determined.
     * @return myDate Date of Easter Monday.
     */
    myDate get_easter_monday(const std::size_t& year);

    /**
     * @brief Get date of Easter Friday.
     * 
     * @param year Year for which the date of Easter Friday should be determined.
     * @return myDate Date of Easter Friday.
     */
    myDate get_easter_friday(const std::size_t& year);

    /**
     * @brief Get date of Ascension day.
     * 
     * @param year Year for which the date of Ascension day should be determined.
     * @return myDate Date of Ascension day.
     */
    myDate get_ascension_day(const std::size_t& year);

    /**
     * @brief Get date of Whit day.
     * 
     * @param year Year for which the date of Whit day should be determined.
     * @return myDate Date of Whit day.
     */
    myDate get_whit_monday(const std::size_t& year);

    /**
     * @brief Function that determines - for example - the first Monday in May of 2023. This function is typically used when determining floating holidays.
     * 
     * @param date_begin Date begin.
     * @param date_end Date end.
     * @param day_in_week Day in week; 1 = Monday, 7 = Sunday.
     * @param rank Number of the respective day in week, e.g. Monday, in the time period determined through begining and end date.
     * @param go_forward true => Start from begining date go towards end date. \n false => Start from end date and go towards begining date.
     * @return myDate Date that meets the user specified condition, e.g. the second Monday in May 2023 (in May => begining date = 01/05/2023 & end date = 31/05/2023).
     */
    myDate get_holiday_day(const myDate& date_begin, const myDate& date_end, const unsigned short& day_in_week, const unsigned short& rank, const bool& go_forward = true);

    /**
     * @brief Get the weekend dates for a certain time period specified through beging and end year.
     * 
     * @param year_begin Begining year.
     * @param year_end End year.
     * @return std::vector<myDate> Vector of weekend dates.
     */
    std::vector<myDate> get_weekends(const std::size_t& year_begin, const std::size_t& year_end);

    /**
     * @brief Get Czech public holidays for a certain time period specified through beging and end year.
     * 
     * @param year_begin Begining year.
     * @param year_end End year.
     * @return std::vector<myDate> Vector of public holdidays dates.
     */
    std::vector<myDate> get_holidays_cz(const std::size_t& year_begin, const std::size_t& year_end);

    /**
     * @brief Get German public holidays for a certain time period specified through beging and end year.
     * 
     * @param year_begin Begining year.
     * @param year_end End year.
     * @return std::vector<myDate> Vector of public holdidays dates.
     */
    std::vector<myDate> get_holidays_de(const std::size_t& year_begin, const std::size_t& year_end);

    /**
     * @brief Get US holidays for a certain time period specified through beging and end year.
     * 
     * @param year_begin Begining year.
     * @param year_end End year.
     * @return std::vector<myDate> Vector of public holdidays dates.
     */
    std::vector<myDate> get_holidays_us(const std::size_t& year_begin, const std::size_t& year_end);

    /**
     * @brief Get UK public holidays for a certain time period specified through beging and end year.
     * 
     * @param year_begin Begining year.
     * @param year_end End year.
     * @return std::vector<myDate> Vector of public holdidays dates.
     */
    std::vector<myDate> get_holidays_uk(const std::size_t& year_begin, const std::size_t& year_end);
}

