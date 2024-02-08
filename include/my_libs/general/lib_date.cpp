#include <string>
#include <iostream>
#include <cmath>
#include <tuple>
#include <vector>
#include <memory>
#include <limits>
#include "lib_date.h"

/*
 * STAND-ALONE FUNCTIONS
 */

// determine number of days in month
std::size_t lib_date::days_in_month(const std::size_t& year, const std::size_t& month)
{
    switch (month)
    {
        // months that have 31 days
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        // months that have 30 days
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        // February
        case 2:
            if (year % 4 == 0)
                return 29;
            else
                return 28;
        // incorrect month
        default:
            throw std::invalid_argument((std::string)__func__ + ": " + std::to_string(month) + "  is not supported month!" );
    }
}

// add months
std::vector<std::size_t> lib_date::add_months(const std::size_t& year, const std::size_t& month, const std::size_t& day, const std::size_t& months_to_add)
{
    // declare array of integers to hold information on year, month and day
    std::vector<std::size_t> date;

    // individual date components
    std::size_t _years_to_add;
    std::size_t _year;
    std::size_t _month;
    std::size_t _day;

    // determine how many years and months should be added straight away
    _years_to_add = months_to_add / 12;
    _month = months_to_add % 12;

    // determine month
    if ((_month > 0) && (_month + month) > 12)
    {
        _years_to_add++;
        _month = (_month + month) % 12;
    }
    else
    {
        _month += month;
    }

    // determine year
    _year = year + _years_to_add;

    // check maximum day in month
    _day = lib_date::days_in_month(_year, _month);
    if (day < _day)
        _day = day;

    // return result
    date.push_back(_year);
    date.push_back(_month);
    date.push_back(_day);

    return date;
}

// remove months
std::vector<std::size_t> lib_date::remove_months(const std::size_t& year, const std::size_t& month, const std::size_t& day, const std::size_t& months_to_remove)
{
    // declare array of integers to hold information on year, month and day
    std::vector<std::size_t> date;

    // individual date components
    std::size_t _years_to_remove;
    std::size_t _year;
    std::size_t _month;
    std::size_t _day;

    // determine how many years and months should be removed straight away
    _years_to_remove = months_to_remove / 12;
    _month = months_to_remove % 12;

    // determine month
    if ((_month > 0) && (month - _month) < 1)
    {
        _years_to_remove++;
        _month = (12 + month - _month);
    }
    else
    {
        _month = month - _month;
    }

    // determine year
    _year = year - _years_to_remove;

    // check maximum day in month
    _day = lib_date::days_in_month(_year, _month);
    if (day < _day)
        _day = day;

    // return result
    date.push_back(_year);
    date.push_back(_month);
    date.push_back(_day);

    return date;
}

// decompose date frequency of (for example) "6M" into 6 and "M"
std::tuple<std::size_t, std::string> lib_date::decompose_freq(const std::string& freq)
{
   // variables to hold frequency type (e.g. D, M, Y) and number of frequency units
   std::size_t freq_no;
   std::string freq_type; 

    // convert O/N and T/N
    if (freq.compare("ON") == 0)
        return {1, "D"};
    else if (freq.compare("TN") == 0)
        return {2, "D"};

   // determine frequency units
   freq_no = stoi(freq.substr(0, freq.size() - 1));

   // determine frequency type
   freq_type = freq.substr(freq.size() - 1, 1);

    // convert weeks to days
    if (freq_type.compare("W") == 0)
    {
        freq_type = "D";
        freq_no *= 7;
    }

   // return number of frequency units and frequency type
   return {freq_no, freq_type};
 }

// create a vector of dates from start date to end date using time step of a given frequency
std::vector<lib_date::myDate> lib_date::create_date_serie(const std::string& date_str_begin, const std::string& date_str_end, const std::string& date_freq, const std::string& date_format)
{
    // create vector to hold date serie
    std::vector<lib_date::myDate> date_serie;

    //create date end and current date objects
    lib_date::myDate date_end(date_str_end, date_format);
    lib_date::myDate date_current(date_str_begin, date_format);

    // create date series
    while (date_current.get_date_int() <= date_end.get_date_int())
    {
        date_serie.push_back(date_current);
        date_current.add(date_freq);
    }

    // return vector with date serie
    return date_serie;
}

// convert date frequency string into approximate year fraction; could
// be used to compare individual date frequencies
double lib_date::eval_freq(const std::string& freq)
{
    // split frequency string into number of frequency units and frequency type
    std::size_t freq_no;
    std::string freq_type;
    std::tie(freq_no, freq_type) = lib_date::decompose_freq(freq);   

    // calculate approximate year fraction representing given frequency
    // string
    if (freq_type.compare("D") == 0)
    {
        return freq_no / 365.25;
    }
    else if (freq_type.compare("M") == 0)
    {
        return 30.438 * freq_no / 365.25;
    }
    else if (freq_type.compare("Y") == 0)
    {
        return freq_no;
    }
    else
    {
        throw std::runtime_error((std::string)__func__ + ": unsupported date frequency type " + freq_type + "!");
    }
}

// get index of the nearest date from the vector of dates; we assume that
// the dates in vector are sorted in ascending order
std::size_t get_nearest_idx(const lib_date::myDate& date, const std::vector<lib_date::myDate>& dates, const std::string& type)
{
    // varibles to hold distance
    std::size_t dist = std::numeric_limits<std::size_t>::max();
    std::size_t nearest_dist = std::numeric_limits<std::size_t>::max();

    // variable to hold the nearest index
    std::size_t nearest_idx = std::numeric_limits<std::size_t>::max();

    // go through the dates in the vector
    for (std::size_t idx = 0; idx < dates.size(); idx++)
    {
        // calculate distance between dates in number of days
        dist = date.get_days_no() - dates[idx].get_days_no();
        
        if (type.compare("abs") == 0)
        {
            if (dist < 0)
                dist *= -1;
        }
        else if (type.compare("smaller") == 0)
        {
            if (dist <= 0)
            {
                dist *= -1;
            }
            else
            {
                dist = std::numeric_limits<std::size_t>::max();
            }
        }
        else if (type.compare("larger") == 0)
        {
            if (dist < 0)
            {
                dist = std::numeric_limits<std::size_t>::max();
            }
        }
        else
        {
            throw std::runtime_error((std::string)__func__ + ": " + type + " is not supported type!");
        }
        
        // the first loop pass
        if (idx == 0)
        {
            nearest_idx = idx;
            nearest_dist = dist;
        }
        // other loop passes
        else
        {
            if (dist < nearest_dist)
            {
                nearest_idx = idx;
                nearest_dist = dist;
            }
        }
    }

    // check that the nearest date from the vector was indeed found (does not
    // have to be the case for "smaller" and "larger" search type)
    if (nearest_idx == std::numeric_limits<std::size_t>::max())
    {
        throw std::runtime_error((std::string)__func__ + ": No nearest date found using search type " + type + "!");
    }

    // return index of the nearest date from the vector
    return nearest_idx;
}

// demetermine Easter Sunday for a given a year
lib_date::myDate lib_date::get_easter_sunday(const std::size_t& year)
{
    // define variables
    std::size_t day;
    std::size_t month;

    std::size_t a;
    std::size_t b;
    std::size_t c;
    std::size_t d;
    std::size_t e;
    std::size_t f;
    std::size_t g;
    std::size_t h;
    std::size_t i;
    std::size_t j;
    std::size_t k;
    std::size_t m;
    std::size_t p;

    // check year range
    if (year < 1 || year > 5000)
        throw std::runtime_error((std::string)__func__ + ": Only year values between 1 and 5,000!");

    // calculate auxiliary variables
    a = year % 19;
    b = year / 100;
    c = year % 100;
    d = b / 4;
    e = b % 4;
    f = (b + 8) / 25;
    g = (b - f + 1) / 3;
    h = ((19 * a) + b - d - g + 15) % 30;
    i = c / 4;
    j = c % 4;
    k = (32 + (2 * e) + (2 * i) - h - j) % 7;
    m = (a + (11 * h) + (22 * k)) / 451;
    p = (h + k - (7 * m) + 114) % 31;

    // determine Easter Sunday date
    month = (h + k - (7 * m) + 114) / 31;
    day = p + 1;

    // return Easter Sunday date
    return lib_date::myDate(year * 10000 + month * 100 + day);
}

// demetermine Easter Monday for a given a year
lib_date::myDate lib_date::get_easter_monday(const std::size_t& year)
{
    lib_date::myDate easter_monday = lib_date::get_easter_sunday(year);
    easter_monday.add("1D");
    return easter_monday;
}

// demetermine Easter Friday for a given a year
lib_date::myDate lib_date::get_easter_friday(const std::size_t& year)
{
    lib_date::myDate easter_friday = lib_date::get_easter_sunday(year);
    easter_friday.remove("2D");
    return easter_friday;
}

// demetermine Ascension Day for a given a year
lib_date::myDate lib_date::get_ascension_day(const std::size_t& year)
{
    lib_date::myDate ascension_day = lib_date::get_easter_sunday(year);
    ascension_day.add("39D");
    return ascension_day;
}

// demetermine Whit Monday for a given a year
lib_date::myDate lib_date::get_whit_monday(const std::size_t& year)
{
    lib_date::myDate whit_monday = lib_date::get_easter_sunday(year);
    whit_monday.add("50D");
    return whit_monday;
}

// function that determines - for example - the first Monday in May of 2023; this function is typically used when determining floating holidays
lib_date::myDate lib_date::get_holiday_day(const lib_date::myDate& date_begin, const lib_date::myDate& date_end, const unsigned short& day_in_week, const unsigned short& rank, const bool& go_forward)
{
    // check that variable 'day_in_week' is between 1 and 7
    if ((day_in_week < 1) || (day_in_week > 7))
        throw std::runtime_error((std::string)__func__ + ": Variable 'day_in_week' must between 1 and 7!");

    // check that variable 'rank' is between 1 and 5
    if ((day_in_week < 1) || (day_in_week > 7))
        throw std::runtime_error((std::string)__func__ + ": No month has more than 5 weeks!");

    // variable that holds auxiliary date
    lib_date::myDate date_aux;

    // variable that holds information about - for example - what Monday in May of 2023 we have
    unsigned short rank_aux = 0;

    // decide if we should start form date_start and go forward or start from date_end and go backwards
    if (go_forward)
    {
        date_aux = date_begin;
        do
        {
            if (date_aux.get_day_in_week() == day_in_week)
            {
                rank_aux++;
                if (rank_aux == rank)
                    return date_aux;
            }
            date_aux.add("1D");
        } while (date_aux <= date_end);
    }
    else
    {
        date_aux = date_end;
        do
        {
            if (date_aux.get_day_in_week() == day_in_week)
            {
                rank_aux++;
                if (rank_aux == rank)
                    return date_aux;
            }
            date_aux.remove("1D");
        } while (date_aux >= date_begin);
    }

    // this row should be note reached; it could happen if variable 'rank' is too high
    throw std::runtime_error((std::string)__func__ + ": Date was not found!");

}

// get list of weekends; could be used as a general list of holidays
std::vector<lib_date::myDate> lib_date::get_weekends(const std::size_t& year_begin, const std::size_t& year_end)
{
    lib_date::myDate date_curt = lib_date::myDate(year_begin * 10000 + 1 * 100 + 1);
    lib_date::myDate date_end = lib_date::myDate(year_end * 10000 + 12 * 100 + 31);
    std::vector<lib_date::myDate> weekends;
    while (date_curt <= date_end)
    {
        // check for weekend
        if ((date_curt.get_day_in_week() == 6) || (date_curt.get_day_in_week() == 7))
            weekends.push_back(date_curt);

        // next day
        date_curt.add("1D");
    }

    // return vector of weekends
    return weekends;

}

// Czech public holidays
std::vector<lib_date::myDate> lib_date::get_holidays_cz(const std::size_t& year_begin, const std::size_t& year_end)
{
    std::vector<lib_date::myDate> holidays;
    std::size_t year_curt = year_begin;
    while (year_curt <= year_end)
    {
        // New Year's Day
        lib_date::myDate new_year = lib_date::myDate(year_curt * 10000 + 1 * 100 + 1);
        holidays.push_back(new_year);

        // Good Friday
        lib_date::myDate good_friday = lib_date::get_easter_friday(year_curt);
        holidays.push_back(good_friday);

        // Easter Monday
        lib_date::myDate easter_monday = lib_date::get_easter_monday(year_curt);
        holidays.push_back(easter_monday);

        // May Day
        lib_date::myDate may_day = lib_date::myDate(year_curt * 10000 + 5 * 100 + 1);
        holidays.push_back(may_day);

        // Liberation Day
        lib_date::myDate liberation_day = lib_date::myDate(year_curt * 10000 + 5 * 100 + 8);
        holidays.push_back(liberation_day);

        // Staints Cyril and Methodius Day
        lib_date::myDate cyril_methodius = lib_date::myDate(year_curt * 10000 + 7 * 100 + 5);
        holidays.push_back(cyril_methodius);

        // Jan Hus Day
        lib_date::myDate jan_hus = lib_date::myDate(year_curt * 10000 + 7 * 100 + 6);
        holidays.push_back(jan_hus);

        // Statehood Day
        lib_date::myDate statehood_day = lib_date::myDate(year_curt * 10000 + 9 * 100 + 28);
        holidays.push_back(statehood_day);

        // Idependence Day
        lib_date::myDate independence_day = lib_date::myDate(year_curt * 10000 + 10 * 100 + 28);
        holidays.push_back(independence_day);

        // Student's day
        lib_date::myDate students_day = lib_date::myDate(year_curt * 10000 + 11 * 100 + 17);
        holidays.push_back(students_day);

        // Christmas Eve
        lib_date::myDate christmas_eve = lib_date::myDate(year_curt * 10000 + 12 * 100 + 24);
        holidays.push_back(christmas_eve);

        // Christmas Day
        lib_date::myDate christmas_day = lib_date::myDate(year_curt * 10000 + 12 * 100 + 25);
        holidays.push_back(christmas_day);

        // Second Christmas Day
        lib_date::myDate second_christmas_day = lib_date::myDate(year_curt * 10000 + 12 * 100 + 26);
        holidays.push_back(second_christmas_day);

        // next year
        year_curt++;
    }

    // return vector of public holidays
    return holidays;

}

// German public holidays (in all Bundeslands)
std::vector<lib_date::myDate> lib_date::get_holidays_de(const std::size_t& year_begin, const std::size_t& year_end)
{
    std::vector<lib_date::myDate> holidays;
    std::size_t year_curt = year_begin;
    while (year_curt <= year_end)
    {
        // New Year's Day
        lib_date::myDate new_year = lib_date::myDate(year_curt * 10000 + 1 * 100 + 1);
        holidays.push_back(new_year);

        // Good Friday
        lib_date::myDate good_friday = lib_date::get_easter_friday(year_curt);
        holidays.push_back(good_friday);

        // Easter Monday
        lib_date::myDate easter_monday = lib_date::get_easter_monday(year_curt);
        holidays.push_back(easter_monday);

        // May Day
        lib_date::myDate may_day = lib_date::myDate(year_curt * 10000 + 5 * 100 + 1);
        holidays.push_back(may_day);

        // Ascension Day
        lib_date::myDate ascension_day = lib_date::get_ascension_day(year_curt);
        holidays.push_back(ascension_day);

        // Whit Monday
        lib_date::myDate whit_monday = lib_date::get_whit_monday(year_curt);
        holidays.push_back(whit_monday);

        // German Unity Day
        lib_date::myDate unity_day = lib_date::myDate(year_curt * 10000 + 10 * 100 + 3);
        holidays.push_back(unity_day);

        // Christmas Day
        lib_date::myDate christmas_day = lib_date::myDate(year_curt * 10000 + 12 * 100 + 25);
        holidays.push_back(christmas_day);

        // Second Christmas Day
        lib_date::myDate second_christmas_day = lib_date::myDate(year_curt * 10000 + 12 * 100 + 26);
        holidays.push_back(second_christmas_day);

        // next year
        year_curt++;
    }

    // return vector of public holidays
    return holidays;

}

// US federal public holidays
std::vector<lib_date::myDate> lib_date::get_holidays_us(const std::size_t& year_begin, const std::size_t& year_end)
{
    std::vector<lib_date::myDate> holidays;
    std::size_t year_curt = year_begin;

    while (year_curt <= year_end)
    {
        // New Year's Day
        {
            lib_date::myDate new_year = lib_date::myDate(year_curt * 10000 + 1 * 100 + 1);
            holidays.push_back(new_year);
        }

        // Martin Luther King
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 1 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 1 * 100 + 31);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 3; // third Monday in January
            bool go_forward = true;
            lib_date::myDate martin_luther_king = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(martin_luther_king);
        }

        // Washington's Birthday
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 2 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 2 * 100 + 28);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 3; // third Monday in January
            bool go_forward = true;
            lib_date::myDate washingtons_birthday = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(washingtons_birthday);
        }

        // Memorial Day
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 5 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 5 * 100 + 31);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 1; // last Monday in May
            bool go_forward = false;
            lib_date::myDate memorial_day = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(memorial_day);
        }

        // Juneteenth National Independence Day
        {
            lib_date::myDate juneteenth_independence_day = lib_date::myDate(year_curt * 10000 + 6 * 100 + 17);
            holidays.push_back(juneteenth_independence_day);
        }

        // Independence Day
        {
            lib_date::myDate independence_day = lib_date::myDate(year_curt * 10000 + 7 * 100 + 4);
            holidays.push_back(independence_day);
        }

        // Labor Day
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 9 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 9 * 100 + 30);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 1; // first Monday in September
            bool go_forward = true;
            lib_date::myDate labor_day = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(labor_day);
        }

        // Columbus Day
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 10 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 10 * 100 + 30);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 2; // second Monday in October
            bool go_forward = true;
            lib_date::myDate columbus_day = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(columbus_day);
        }

        // Veterans Day
        {
            lib_date::myDate veterans_day = lib_date::myDate(year_curt * 10000 + 11 * 100 + 11);
            holidays.push_back(veterans_day);
        }

        // Thanksgiving Day
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 11 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 11 * 100 + 30);
            std::size_t day_in_week = 4; // Thursday
            std::size_t rank = 4; // fourth Thursday in November
            bool go_forward = true;
            lib_date::myDate thanksgiving_day = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(thanksgiving_day);
        }

        // Christmas Day
        {
            lib_date::myDate christmas_day = lib_date::myDate(year_curt * 10000 + 12 * 100 + 25);
            holidays.push_back(christmas_day);
        }

        // next year
        year_curt++;
    }

    // return vector of public holidays
    return holidays;

}

// UK public holidays
std::vector<lib_date::myDate> lib_date::get_holidays_uk(const std::size_t& year_begin, const std::size_t& year_end)
{
    std::vector<lib_date::myDate> holidays;
    std::size_t year_curt = year_begin;

    while (year_curt <= year_end)
    {
        // New Year's Day
        {
            lib_date::myDate new_year = lib_date::myDate(year_curt * 10000 + 1 * 100 + 1);

            // New Year's Day on Saturday
            if (new_year.get_day_in_week() == 6)
                new_year.add("2D");
            // New Year's Day on Sunday
            else if (new_year.get_day_in_week() == 7)
                new_year.add("1D");

            holidays.push_back(new_year);
        }

        // Good Friday
        {
            lib_date::myDate good_friday = lib_date::get_easter_friday(year_curt);
            holidays.push_back(good_friday);
        }

        // Easter Monday
        {
            lib_date::myDate easter_monday = lib_date::get_easter_monday(year_curt);
            holidays.push_back(easter_monday);
        }

        // Early May Bank Holiday
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 5 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 5 * 100 + 30);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 1; // first Monday in May
            bool go_forward = true;
            lib_date::myDate first_bank_holiday = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(first_bank_holiday);
        }

        // Spring Bank Holiday
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 5 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 5 * 100 + 30);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 1; // last Monday in May
            bool go_forward = false;
            lib_date::myDate second_bank_holiday = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(second_bank_holiday);
        }

        // August Bank Holiday
        {
            lib_date::myDate date_begin = lib_date::myDate(year_curt * 10000 + 8 * 100 + 1);
            lib_date::myDate date_end = lib_date::myDate(year_curt * 10000 + 8 * 100 + 31);
            std::size_t day_in_week = 1; // Monday
            std::size_t rank = 1; // last Monday in August
            bool go_forward = false;
            lib_date::myDate third_bank_holiday = lib_date::get_holiday_day(date_begin, date_end, day_in_week, rank, go_forward);
            holidays.push_back(third_bank_holiday);
        }

        // Christmas Day & Boxing Day
        {
            lib_date::myDate christmas_day = lib_date::myDate(year_curt * 10000 + 12 * 100 + 25);
            lib_date::myDate boxing_day = lib_date::myDate(year_curt * 10000 + 12 * 100 + 26);

            // Christmas Day on Friday
            if (christmas_day.get_day_in_week() == 5)
                boxing_day.add("2D");
            // Christmas Day on Saturday
            else if (christmas_day.get_day_in_week() == 6)
            {
                christmas_day.add("2D");
                boxing_day.add("2D");
            }
            // Christmas Day on Sunday
            else if (christmas_day.get_day_in_week() == 7) // Sunday
            {
                christmas_day.add("1D");
                boxing_day.add("1D");
            }

            holidays.push_back(christmas_day);
            holidays.push_back(boxing_day);

        }

        // next year
        year_curt++;
    }

    // return vector of public holidays
    return holidays;

}


/*
 * OBJECT FUNCTIONS
 */

// set year, month and day based on date std::string in yyyymmdd format
void lib_date::myDate::set_year_month_day()
{
    // determine year, month and day
    year = (std::size_t)stoi(this->date_str.substr(0, 4));
    month = (std::size_t)stoi(this->date_str.substr(4, 2));
    day = (std::size_t)stoi(this->date_str.substr(6, 2));
}

// derive date "distance" in days from initial date of 01/01/1601
void lib_date::myDate::set_days_no()
{
    // calculate number of days for initial date
        
        // years contribution
        this->days_no = (year - init_year) * 365;
        this->days_no += (year - init_year) / 4; // leap years
        this->days_no -= (year - init_year) / 100; // we skip leap years every 100 years
        this->days_no += (year - init_year) / 400; // unless the year is divisible by 400
        
        // months contribution
        for (std::size_t i = month - 1; i > 0; --i)
        {
            days_no += days_in_month(year, i);
        }
        
        // days contribution
        days_no += day - 1;
}

// derive day in week knowning that 01/01/1601 (this->days_no = 0) is Tuesday
void lib_date::myDate::set_day_in_week()
{
    // check Tuesday
    if ((this->days_no - 1) % 7 == 0)
        this->day_in_week = 2;
    // check Wednesday
    else if ((this->days_no - 2) % 7 == 0)
         this->day_in_week = 3;
    // check Thursday
    else if ((this->days_no - 3) % 7 == 0)
         this->day_in_week = 4;
    // check Friday
    else if ((this->days_no - 4) % 7 == 0)
         this->day_in_week = 5;
    // check Saturday
    else if ((this->days_no - 5) % 7 == 0)
         this->day_in_week = 6;
    // check Sunday
    else if ((this->days_no - 6) % 7 == 0)
         this->day_in_week = 7;
    // check Monday
    else if ((this->days_no - 7) % 7 == 0)
         this->day_in_week = 1;
    // something wrong
    else
        throw std::runtime_error((std::string)__func__ + ": '" + this->get_date_str() + "'  date cannot be converted into day in week!");     
}

// update date integer, year, month and day based on date std::string in format yyyymmdd
void lib_date::myDate::recalc()
{
    // convert yyyymmdd date std::string into integer
    this->date_int = (std::size_t)stoi(date_str);

    // set year, month and day
    this->set_year_month_day();

    // set days_no
    this->set_days_no();

    // set day_in_week
    this->set_day_in_week();
}

// set myDate using string date
void lib_date::myDate::set(const std::string& date_str, const std::string& date_format)
{
    // convert date std::string into yyyymmdd string format
    if (date_format.compare("dd/mm/yyyy") == 0)
    {
        this->date_str = date_str.substr(6,4) + date_str.substr(3,2) + date_str.substr(0, 2);
    }
    else if (date_format.compare("yyyy/mm/dd") == 0)
    {
        this->date_str = date_str.substr(0,4) + date_str.substr(5,2) + date_str.substr(8, 2);
    }
    else if (date_format.compare("ddmmyyyy") == 0)
    {
        this->date_str = date_str.substr(4, 4) + date_str.substr(2, 2) + date_str.substr(0, 2);
    }
    else if (date_format.compare("yyyymmdd") == 0)
    {
        this->date_str = date_str;
    }
    else
    {
        throw std::invalid_argument((std::string)__func__ + ": " + date_format + " is not a supported date string format!");
    }

    // determine year, month, day and number of days since 01/01/1601
    this->recalc();

}

// set myDate using integer date
void lib_date::myDate::set(const std::size_t& date_int)
{
    // store date integer; we assume yyyymmdd format
    this->date_int = date_int;

    // store date as std::string in yyyymmdd
    this->date_str = std::to_string(date_int);

    // determine year, month, day and number of days since 01/01/1601
    this->recalc();
}

//shift date forward using specified date frequency (e.g. 2D, 3M, 10Y)
void lib_date::myDate::add(const std::string& date_freq)
{
    // variables to hold frequency type (e.g. D, M, Y) and number of frequency units
    std::size_t date_freq_no;
    std::string date_freq_type;
    std::tie(date_freq_no, date_freq_type) = decompose_freq(date_freq); 

    if (date_freq_type.compare("D") == 0)
    {
        std::size_t _days_no;
        std::size_t _years_to_add;
        std::size_t _months_to_add;
        std::size_t _days_in_month;
        std::string _freq_date;

        // store number of days in the current date
        _days_no = this->days_no;

        // add years to the current days
        _years_to_add = date_freq_no / 366;
        if (_years_to_add != 0)
        {
            _days_no = this->get_days_no();
            _freq_date = std::to_string(_years_to_add) + "Y";
            this->add(_freq_date);
            date_freq_no -= this->days_no - _days_no;
        }

        // add months in bulk
        _months_to_add = date_freq_no / 31;
        if (_months_to_add != 0)
        {
            _days_no = this->get_days_no();
            _freq_date = std::to_string(_months_to_add) + "M";
            this->add(_freq_date);
            date_freq_no -= this->days_no - _days_no;
        }

        // add the last one or two months
        _days_in_month = days_in_month(this->year, this->month);
        while (date_freq_no + this->day > _days_in_month)
        {
            _days_no = this->get_days_no();
            _freq_date = "1M";
            this->add(_freq_date);
            date_freq_no -= this->days_no - _days_no;
            _days_in_month = days_in_month(this->year, this->month);
        }
        
        // add remaining days
        this->day += date_freq_no;
        this->date_str = std::to_string(this->year * 10000 + this->month * 100 + this->day);
        this->recalc();

    }
    else if (date_freq_type.compare("M") == 0)
    {
        std::vector<std::size_t>date = add_months(this->year, this->month, this->day, date_freq_no);
        this->year = date[0];
        this->month = date[1];
        this->day = date[2];
        this->date_str = std::to_string(this->year * 10000 + this->month * 100 + this->day);
        this->recalc();
    }
    else if (date_freq_type.compare("Y") == 0)
    {
        this->year += date_freq_no;
        this->date_str = std::to_string(this->year * 10000 + this->month * 100 + this->day);
        this->recalc();
    }
    else
    {
        throw std::invalid_argument((std::string)__func__ + ": " + date_freq_type + " is  notsupported date date frequency type!");
    }
}

//shift date forward using specified date frequency (e.g. 2D, 3M, 10Y)
void lib_date::myDate::remove(const std::string& date_freq)
{
    // variables to hold frequency type (e.g. D, M, Y) and number of frequency units
    std::size_t date_freq_no;
    std::string date_freq_type;

    tie(date_freq_no, date_freq_type) = decompose_freq(date_freq);

    if (date_freq_type.compare("D") == 0)
    {
        std::size_t _days_no;
        std::size_t _years_to_remove;
        std::size_t _months_to_remove;
        std::size_t _days_in_month;
        std::string _freq_date;

        // store number of days in the current date
        _days_no = this->days_no;

        // remove years from the current days
        _years_to_remove = date_freq_no / 365;
        if (_years_to_remove != 0)
        {
            _days_no = this->get_days_no();
            _freq_date = std::to_string(_years_to_remove) + "Y";
            this->remove(_freq_date);
            date_freq_no -= _days_no - this->days_no;
        }

        // remove months in bulk
        _months_to_remove = date_freq_no / 30;
        if (_months_to_remove != 0)
        {
            _days_no = this->get_days_no();
            _freq_date = std::to_string(_months_to_remove) + "M";
            this->remove(_freq_date);
            date_freq_no -= _days_no - this->days_no;
        }

        // remove the last one or two months
        while (this->day - date_freq_no < 0)
        {
            date_freq_no -= this->day;
            this->month--;
            _days_in_month = days_in_month(this->year, this->month);
            this->day = _days_in_month;
            if (this->month < 0)
            {
                this->year--;
                this->month += 12;
            }
            this->date_str = std::to_string(this->year * 10000 + this->month * 100 + this->day);
            this->recalc();
        }
        this->day -= date_freq_no;
        this->date_str = std::to_string(this->year * 10000 + this->month * 100 + this->day);
        this->recalc();            
    }
    else if (date_freq_type.compare("M") == 0)
    {
        std::vector<std::size_t>date = remove_months(this->year, this->month, this->day, date_freq_no);
        this->year = date[0];
        this->month = date[1];
        this->day = date[2];
        this->date_str = std::to_string(this->year * 10000 + this->month * 100 + this->day);
        this->recalc();
    }
    else if (date_freq_type.compare("Y") == 0)
    {
        year -= date_freq_no;
        this->date_str = std::to_string(this->year * 10000 + this->month * 100 + this->day);
        this->recalc();
    }
    else
    {
        throw std::invalid_argument((std::string)__func__ + ": " + date_freq_type + " is  notsupported date date frequency type!");
    }
}

bool lib_date::myDate::is_last_day_in_month() const
{
    // copy myDate object into an auxiliary variable
    lib_date::myDate date_aux = myDate(this->get_date_int());

    // store the current month into an auxiliary variable
    int month_aux = date_aux.get_month();

    // add one date to the date
    date_aux.add("1D");

    // check if the month has changed
    if (date_aux.get_month() > month_aux){
        return true;
    }
    else
    {
        return false;
    }
}

bool lib_date::myDate::is_leap_year() const
{
    // variables
    bool is_leap = false;
    int year = this->get_year();

    // the most common defintion of a leap year is divisibility by 4
    if (year % 4 == 0)
    {
        is_leap = true;
    }

    // however we skip leap year every 100 years
    if (year % 100 == 0)
    {
        is_leap = false;
    }

    // unless the year is not divisible by 400
    if (year % 400 == 0)
    {
        is_leap = false;
    }

    // return information if the year is leap year or not
    return is_leap;
}

bool lib_date::myDate::is_weekend() const
{
    if ((this->day_in_week == 6) or (this->day_in_week == 7))
        return true;
    else
        return false;
}

bool lib_date::myDate::is_holidays(const std::vector<lib_date::myDate>& holidays) const
{
    // go through a vector with holiday dates
    for (unsigned short idx = 0; idx < holidays.size(); idx++)
    {
        if (this->days_no == holidays[idx].get_days_no())
            return true;
    }

    return false;

}

bool lib_date::myDate::is_working_day(const std::vector<lib_date::myDate>& holidays) const
{
    // check for weekend
    if ((this->day_in_week == 6) || (this->day_in_week == 7))
        return false;

    // go through a vector with holiday dates
    for (unsigned short idx = 0; idx < holidays.size(); idx++)
    {
        if (this->days_no == holidays[idx].get_days_no())
            return false;
    }

    return true;
}

void lib_date::myDate::add_working_days(const std::vector<lib_date::myDate>& holidays, const std::size_t& working_days_no)
{
    for (unsigned short working_days = 1; working_days <= working_days_no; working_days++)
        this->set_to_next_working_day(holidays);
}

void lib_date::myDate::set_to_next_working_day(const std::vector<lib_date::myDate>& holidays)
{
    do
    {
        this->add("1D");
    } while ((this->is_weekend()) || (this->is_holidays(holidays)));
}

void lib_date::myDate::set_to_prev_working_day(const std::vector<lib_date::myDate>& holidays)
{
    do
    {
        this->remove("1D");
    } while ((this->is_weekend()) || (this->is_holidays(holidays)));
}

/*
 * OBJECT CONSTRUCTORS
 */

lib_date::myDate::myDate()
{
    this->date_str = "16010101";
    this->date_int = 16010101;
    this->year = 1601;
    this->month = 1;
    this->day = 1;
    this->days_no = 0;
    this->day_in_week = 2;
};

lib_date::myDate::myDate(const std::string& date_str, const std::string& date_format)
{
    this->set(date_str, date_format);
}

lib_date::myDate::myDate(const std::size_t& date_int)
{
    this->set(date_int);
}

/*
 * COPY CONSTRUCTOR
 */

lib_date::myDate::myDate (const lib_date::myDate& date)
{
    this->date_str = date.get_date_str();
    this->recalc();
};

/*
 * OBJECT OPERATORS
 */

long lib_date::myDate::operator- (const lib_date::myDate &date2)
{
    return days_no - date2.get_days_no();
}

lib_date::myDate& lib_date::myDate::operator= (const lib_date::myDate &date)
{
    this->date_str = date.get_date_str();
    this->recalc();
    return *this;
};

bool lib_date::myDate::operator== (const lib_date::myDate &date)
{
    if (this->days_no == date.days_no)
        return true;
    else
        return false;
};

bool lib_date::myDate::operator< (const lib_date::myDate &date)
{
    if (this->days_no < date.days_no)
        return true;
    else
        return false;
};

bool lib_date::myDate::operator> (const lib_date::myDate &date)
{
    if (this->days_no > date.days_no)
        return true;
    else
        return false;
};

bool lib_date::myDate::operator<= (const lib_date::myDate &date)
{
    if (this->days_no <= date.days_no)
        return true;
    else
        return false;
};

bool lib_date::myDate::operator>= (const lib_date::myDate &date)
{
    if (this->days_no >= date.days_no)
        return true;
    else
        return false;
};