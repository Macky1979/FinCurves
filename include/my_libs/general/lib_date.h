/*
#include <string>
#include <iostream>
#include <memory>
#include "lib_date.h"

int main()
{
    // introduction of the myDate object
    {
        std::cout << "INTRODUCTION OF myDate OBJECT" << std::endl;
        std::string date_str = "30/12/1979";
        std::string date_format = "dd/mm/yyyy";
        lib_date::myDate my_birth(date_str, date_format);
        std::cout << "I was born " + std::to_string(my_birth.get_days_no()) + " days after 01/01/1601." << std::endl;
        std::cout << '\n' << std::endl;
    }

    // overloaded operators
    {
        std::cout << "OVERLOADED OPERATOR" << std::endl;
        lib_date::myDate my_birth(19791230);
        lib_date::myDate today(20230209);
        std::cout << today - my_birth << " days being alive!" << std::endl;
        lib_date::myDate some_day;
        some_day = today;
        std::cout << "Today is " + some_day.get_date_str() + "." << std::endl;
        std::cout << '\n' << std::endl;
    }

    // copy construtor and function add()
    {
        std::cout << "COPY CONSTRUCTOR AND FUNCTION ADD()" << std::endl;
        lib_date::myDate my_birth(19791230);
        lib_date::myDate birthsday = my_birth;
        birthsday.add("40Y");
        std::cout << "I was born on " + my_birth.get_date_str() + "." << std::endl;
        std::cout << "I was 40Y on " + birthsday.get_date_str() + "." << std::endl;
        birthsday.add("22M");
        std::cout << "Today I am " + std::to_string((birthsday - my_birth) / 365.) + " years old." << std::endl;
        std::cout << '\n' << std::endl;
    }

    // function remove
    {
        std::cout << "FUNCTION REMOVE()" << std::endl;
        lib_date::myDate yesterday = lib_date::myDate(20211106);
        yesterday.remove("253M");
        std::cout << "253 months before 20211106 was " << yesterday.get_date_str() + "." << std::endl;
        std::cout << '\n' << std::endl;
    }

    // smart pointer to myDate object
    {
        std::cout << "SMART POINTER" << std::endl;
        std::shared_ptr<lib_date::myDate> christmas = std::make_shared<lib_date::myDate>(20231224);
        std::cout << "Next X-mas is on " + christmas->get_date_str() + "." << std::endl;
        std::cout << '\n' << std::endl;
    }

    // default constructor and create_date_serie() function
    {
        std::cout << "DEFAULT CONSTRUCTOR AND CREATE_DATE_SERIES() FUNCTION" << std::endl;
        lib_date::myDate renesaince;
        std::cout << "The lowest possible date is " + renesaince.get_date_str() + "." << std::endl;

        // all days in my life; demonstration of create_date_series() function
        std::string date_str_begin = "19791230";
        std::string date_str_end = "20211107";
        std::string date_freq = "1D";
        std::string date_format = "yyyymmdd";
        std::vector<lib_date::myDate> my_life = lib_date::create_date_serie(date_str_begin, date_str_end, date_freq, date_format);
        std::cout << "I am alive " + std::to_string(my_life.size()) + " days!" << std::endl;
        std::cout << "Do you want to list all the days? (y/n)";
        std::string answer;
        std::cin >> answer;
        if ((answer.compare("y") == 0) || (answer.compare("Y") == 0))
        {
            for (unsigned short i = 0; i < my_life.size(); i++)
            {
                std::cout << my_life[i].get_date_str() << '\n';
            }
        }
        std::cout << '\n' << std::endl;
    }

    // Easter related functions
    {
        std::cout << "EASTER RELATED FUNCTIONS" << std::endl;

        std::size_t year = 2023;
        std::cout << "Easter Friday:" << lib_date::get_easter_friday(year).get_date_str() << std::endl;
        std::cout << "Easter Monday:" << lib_date::get_easter_monday(year).get_date_str() << std::endl;
        std::cout << "Ascension Day:" << lib_date::get_ascension_day(year).get_date_str() << std::endl;

        std::cout << '\n' << std::endl;
    }

    // the second and the last Monday in May 2023
    {
        std::cout << "THE SECOND AND THE LAST MONDAY IN MAY 2023" << std::endl;

        bool go_forward;
        lib_date::myDate date_begin(20230501);
        lib_date::myDate date_end(20230531);
        unsigned short day_in_week = 1;  // Monday
        unsigned short rank = 1; // the first or the last day_in_week

        // the first Monday
        go_forward = true;
        std::cout << "The first Monday in May 2023:" << lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward).get_date_str() << std::endl;

        // the last Monday
        go_forward = false;
        std::cout << "The last Monday in May 2023:" << lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward).get_date_str() << std::endl;

        std::cout << '\n' << std::endl;
    }

    // Czech public holidays
    {
        std::cout << "CZECH PUBLIC HOLIDAYS IN 2021 - 2022" << std::endl;
        std::vector<lib_date::myDate> holidays = lib_date::get_holidays_cz(2021, 2022);
        for (unsigned short i = 0; i < holidays.size(); i++)
            std::cout << holidays[i].get_date_str() << std::endl;

        std::cout << '\n' << std::endl;
    }

    // Check for date for weekend, holidays and working date
    {
        std::cout << "CHECK DATE FOR WEEKEND, HOLIDAYS AND WORKING DATE" << std::endl;
        lib_date::myDate some_date = lib_date::myDate(20231216);
        std::vector<lib_date::myDate> holidays = lib_date::get_holidays_de(2023, 2023);

        // check for weekend
        if (some_date.is_weekend())
            std::cout << some_date.get_date_str() << " is weekend" << std::endl;
        else
            std::cout << some_date.get_date_str() << " is not weekend" << std::endl;

        // check for holidays
        if (some_date.is_holidays(holidays))
            std::cout << some_date.get_date_str() << " is holidays in Germany" << std::endl;
        else
            std::cout << some_date.get_date_str() << " is not holidays in Germany" << std::endl;

        // check for working day
        if (some_date.is_working_day(holidays))
            std::cout << some_date.get_date_str() << " is working day in Germany" << std::endl;
        else
            std::cout << some_date.get_date_str() << " is not working day in Germany" << std::endl;

        std::cout << '\n' << std::endl;
    }

    // next and previous working day
    {
        std::cout << "NEXT AND PREVIOUS WORKING DAY" << std::endl;

        // get list of US federal holidays in 2024
        std::vector<lib_date::myDate> holidays = lib_date::get_holidays_us(2024, 2024);

        // next business day
        lib_date::myDate next_working_day = lib_date::myDate(20240112);
        std::cout << "Next working day after " << next_working_day.get_date_str();
        next_working_day.set_to_next_working_day(holidays);
        std::cout << " is " << next_working_day.get_date_str() << std::endl;

        // previous business day
        lib_date::myDate prev_working_day = lib_date::myDate(20240112);
        std::cout << "Previous working day before " << prev_working_day.get_date_str();
        prev_working_day.set_to_prev_working_day(holidays);
        std::cout << " is " << prev_working_day.get_date_str() << std::endl;

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}
*/

#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace lib_date
{
    // define date class
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
            // object constructors
            myDate();
            myDate(const std::string& date_str, const std::string& date_format);
            myDate(const std::size_t& date_int);

            // copy constructor
            myDate (const myDate& date);

            // object destructor
            ~myDate(){};

            // overloaded operators
            long operator- (const myDate& date2);
            myDate& operator= (const myDate& _date);
            bool operator== (const myDate& _date);
            bool operator< (const myDate& date);
            bool operator> (const myDate& date);
            bool operator<= (const myDate& date);
            bool operator>= (const myDate& date);

            // object function declarations
            void recalc();
            void set(const std::string& date_str, const std::string& date_format);
            void set(const std::size_t& date_int);
            std::size_t get_year() const {return year;}
            std::size_t get_month() const {return month;}
            std::size_t get_day() const {return day;}
            std::size_t get_days_no() const {return days_no;}
            unsigned short get_day_in_week() const {return day_in_week;}
            std::size_t get_date_int() const {return date_int;}
            std::string get_date_str() const {return date_str;}
            void add(const std::string& date_freq);
            void remove(const std::string& date_freq);
            bool is_last_day_in_month() const;
            bool is_leap_year() const;
            bool is_weekend() const;
            bool is_holidays(const std::vector<myDate>& holidays) const;
            bool is_working_day(const std::vector<myDate>& holidays) const;
            void add_working_days(const std::vector<myDate>& holidays, const std::size_t& working_days_no = 1);
            void set_to_next_working_day(const std::vector<myDate>& holidays);
            void set_to_prev_working_day(const std::vector<myDate>& holidays);
    };

    // stand-alone functions
    std::size_t days_in_month(const std::size_t& year, const std::size_t& month);
    std::vector<std::size_t> add_months(const std::size_t& year, const std::size_t& month, const std::size_t& day, const std::size_t& months_to_add);
    std::vector<std::size_t> remove_months(const std::size_t& year, const std::size_t& month, const std::size_t& day, const std::size_t& months_to_remove);
    std::tuple<std::size_t, std::string> decompose_freq(const std::string& freq);
    double eval_freq(const std::string& freq);
    std::vector<myDate> create_date_serie(const std::string& date_str_begin, const std::string& date_str_end, const std::string& date_freq, const std::string& date_format = "yyyymmdd");
    std::size_t get_nearest_idx(const myDate& date, const std::vector<myDate>& dates, const std::string& type);
    myDate get_easter_sunday(const std::size_t& year);
    myDate get_easter_monday(const std::size_t& year);
    myDate get_easter_friday(const std::size_t& year);
    myDate get_ascension_day(const std::size_t& year);
    myDate get_whit_monday(const std::size_t& year);
    myDate get_holiday_day(const myDate& date_begin, const myDate& date_end, const unsigned short& day_in_week, const unsigned short& rank, const bool& go_forward = true);
    std::vector<myDate> get_weekends(const std::size_t& year_begin, const std::size_t& year_end);
    std::vector<myDate> get_holidays_cz(const std::size_t& year_begin, const std::size_t& year_end);
    std::vector<myDate> get_holidays_de(const std::size_t& year_begin, const std::size_t& year_end);
    std::vector<myDate> get_holidays_us(const std::size_t& year_begin, const std::size_t& year_end);
    std::vector<myDate> get_holidays_uk(const std::size_t& year_begin, const std::size_t& year_end);
}

