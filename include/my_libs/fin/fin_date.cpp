#include <string>
#include <algorithm>
#include "fin_date.h"

// calculate year fractions for family of 30/360 methods
double calc_year_fraction(const int& day1, const int& month1, const int& year1, const int& day2, const int& month2, const int& year2)
{
        // calculate year fraction
        return (360. * (year2 - year1) + 30. * (month2 - month1) + (day2 - day1)) / 360.;
}

// implementation of day count method; see https://en.wikipedia.org/wiki/Day_count_convention
double fin_date::day_count_method(const lib_date::myDate& date1, const lib_date::myDate& date2, const std::string& dcm)
{
    // variables
    int day1;
    int day2;
    int month1;
    int month2;
    int year1;
    int year2;
    double year_fraction;
    bool last_february_day1;
    bool last_february_day2;

    // check that date1 preceeds date2
    if (date1.get_days_no() >= date2.get_days_no())
    {
        throw std::invalid_argument((std::string)__func__ + ": Parameter date1 must preceed date2!");
    }

    // extract day, month and year from date 1
    day1 = date1.get_day();
    month1 = date1.get_month();
    year1 = date1.get_year();

    // extract day, month and year from date 2
    day2 = date2.get_day();
    month2 = date2.get_month();
    year2 = date2.get_year();

    // 30/360 bond basis
    if (dcm.compare("30_360") == 0)
    {
        // adjust day1
        day1 = std::min(day1, 30);
    
        // adjust day2
        if (day1 > 29)
        {
            day2 = std::min(day2, 30);
        }

        // calculate year fraction
        year_fraction = calc_year_fraction(day1, month1, year1, day2, month2, year2);
    }
    // 30/360 US
    else if (dcm.compare("30US_360") == 0)
    {

        // determine if date1 is the last February day
        if ((month1 == 2) and date1.is_last_day_in_month())
        {
            last_february_day1 = true;
        }
        else
        {
            last_february_day1 = false;
        }

        // determine if date2 is the last February day
        if ((month2 == 2) and date2.is_last_day_in_month())
        {
            last_february_day2 = true;
        }
        else
        {
            last_february_day2 = false;
        }

        // adjust day2 if both date1 and date2 are the last February days
        if (last_february_day1 && last_february_day2)
        {
            day2 = 30;
        }

        // adjust day1 if date1 is the last February day
        if (last_february_day1)
        {
            day1 = 30;
        }

        // adjust day2
        if (day2 == 31 && ((day1 == 30) || (day1 == 31)))
        {
            day2 = 30;
        }

        // adjust day1
        if (day1 == 31)
        {
            day1 = 30;
        }

        // calculate year fraction
        year_fraction = calc_year_fraction(day1, month1, year1, day2, month2, year2);
    }
    // actual / 360 method
    else if (dcm.compare("ACT_360") == 0)
    {
        // calculate year fraction
        year_fraction = (date2.get_days_no() - date1.get_days_no()) / 360.;
    }
    // actual / 365 method
    else if (dcm.compare("ACT_365") == 0)
    {
        // calculate year fraction
        year_fraction = (date2.get_days_no() - date1.get_days_no()) / 365.;
    }
    // actual / actual ISDA method
    else if (dcm.compare("ACT_ACT") == 0)
    {
        // date1 and date2 fall into the same year
        if (year1 == year2)
        {
            // the year is a leap year
            if (date1.is_leap_year())
            {
                // calculate year fraction
                year_fraction = (date2.get_days_no() - date1.get_days_no()) / 366.;           
            }
            else
            {
                // calculate year fraction
                year_fraction = (date2.get_days_no() - date1.get_days_no()) / 365.; 
            }
        }
        // date1 and date2 fall into different years
        else
        {   
            // auxiliary variables
            double year_fraction1_aux;
            double year_fraction2_aux;

            // create auxiliary dates
            lib_date::myDate date1_aux = lib_date::myDate(date1.get_year() * 10000 + 1231);
            lib_date::myDate date2_aux = lib_date::myDate(date2.get_year() * 10000 + 101);

            // calculate the first auxiliary year fraction
            year_fraction1_aux = date1_aux.get_days_no() - date1.get_days_no();
            if (date1.is_leap_year())
            {
                year_fraction1_aux /= 366;
            }
            else
            {
                year_fraction1_aux /= 365;
            }

            // calculate the seconds auxiliary year fraction
            year_fraction2_aux = date2.get_days_no() - date2_aux.get_days_no();
            if (date2.is_leap_year())
            {
                year_fraction2_aux /= 366;
            }
            else
            {
                year_fraction2_aux /= 365;
            }

            // calculate the final year fraction
            year_fraction = year_fraction1_aux + year_fraction2_aux + (date2.get_year() - date1.get_year() - 1);
        }
    }
    // supported day count method
    else
    {
        throw std::invalid_argument((std::string)__func__ + ": " + dcm + " is not a supported day count method!");
    }

    // return calculated year fraction
    return year_fraction;
}

// implementation of date rolling; see https://en.wikipedia.org/wiki/Date_rolling
void fin_date::date_rolling(lib_date::myDate& date, const std::vector<lib_date::myDate>& holidays, const std::string& drm = "following")
{
    // following business day
    if (drm.compare("following") == 0)
    {
        if (!date.is_working_day(holidays))
            date.set_to_next_working_day(holidays);
    }
    else if (drm.compare("modified_following") == 0)
    {
        if (!date.is_working_day(holidays))
        {
            int month = date.get_month();
            date.set_to_next_working_day(holidays);
            if (date.get_month() != month)
                date.set_to_prev_working_day(holidays);
        }
    }
    else if (drm.compare("previous") == 0)
    {
        if (!date.is_working_day(holidays))
                date.set_to_prev_working_day(holidays);
    }
    else if (drm.compare("modified_previous") == 0)
    {
        if (!date.is_working_day(holidays))
        {
            int month = date.get_month();
            date.set_to_prev_working_day(holidays);
            if (date.get_month() != month)
                date.set_to_next_working_day(holidays);
        }
    }
    else
        throw std::runtime_error((std::string)__func__ + ": '" + drm + "' is not a supported date rolling method!");
}

// implementation of date rolling; see https://en.wikipedia.org/wiki/Date_rolling
void fin_date::date_rolling(std::vector<lib_date::myDate>& dates, const std::vector<lib_date::myDate>& holidays, const std::string& drm = "following")
{
    // go date by date and roll it if necessary
    for (unsigned short idx = 0; idx < dates.size(); idx++)
        date_rolling(dates[idx], holidays, drm);
}

// implementation of date rolling; see https://en.wikipedia.org/wiki/Date_rolling
std::vector<std::tuple<lib_date::myDate, lib_date::myDate>> fin_date::create_date_serie(const std::string& date_str_begin, const std::vector<std::string>& date_freqs, const std::string& cnty, const std::string& drm, const std::string& date_format)
{
    // auxiliary dates
    lib_date::myDate date_begin = lib_date::myDate(date_str_begin, date_format);
    lib_date::myDate date_ref = lib_date::myDate(date_str_begin, date_format);

    // list holding final date series
    std::vector<std::tuple<lib_date::myDate, lib_date::myDate>> dates;

    // determine holidays calendar
    std::size_t year_begin = date_begin.get_year();
    std::size_t year_end = year_begin + 100;
    std::vector<lib_date::myDate> holidays;

    if (cnty.compare("cz") == 0)
        holidays = lib_date::get_holidays_cz(year_begin, year_end);
    else if (cnty.compare("de") == 0)
        holidays = lib_date::get_holidays_de(year_begin, year_end);
    else if (cnty.compare("uk") == 0)
        holidays = lib_date::get_holidays_uk(year_begin, year_end);
    else if (cnty.compare("us") == 0)
        holidays = lib_date::get_holidays_us(year_begin, year_end);
    else
        holidays = lib_date::get_weekends(year_begin, year_end);

    // go through date frequencies
    for (unsigned short idx = 0; idx < date_freqs.size(); idx++)
    {
        // process O/N
        if (date_freqs[idx].compare("ON") == 0)
        {
            lib_date::myDate date = date_begin;
            date.add_working_days(holidays, 1);
            dates.push_back({date_begin, date});
            date_ref.set(date.get_date_int());
        // process T/N
        }
        else if (date_freqs[idx].compare("TN") == 0)
        {
            lib_date::myDate date = lib_date::myDate(date_begin.get_date_int());
            date.add_working_days(holidays, 2);
            dates.push_back({date_begin, date});
            date_ref.set(date.get_date_int());
        }
        // process other date frequency strings
        else
        {
            // check for frequency string like 1M-3M
            if (date_freqs[idx].find("-") != std::string::npos)
            {
                std::size_t pos = date_freqs[idx].find("-");

                std::string date_freq_1 = date_freqs[idx].substr(0, pos);
                lib_date::myDate date_1 = lib_date::myDate(date_ref.get_date_int());
                date_1.add(date_freq_1);

                std::string date_freq_2 = date_freqs[idx].substr(pos + 1);
                lib_date::myDate date_2 = lib_date::myDate(date_ref.get_date_int());
                date_2.add(date_freq_2);

                dates.push_back({date_1, date_2});
            }
            // check for frequency string like 1M
            else
            {
                lib_date::myDate date = lib_date::myDate(date_ref.get_date_int());
                date.add(date_freqs[idx]);
                dates.push_back({date_ref, date});
            }
        }
    }

    // roll the dates
    for (unsigned short idx = 0; idx < dates.size(); idx++)
        date_rolling(std::get<1>(dates[idx]), holidays, drm);

    // return date serie
    return dates;

}