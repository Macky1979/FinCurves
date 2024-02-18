#include <string>
#include <iostream>
#include <memory>
#include "lib_date.h"
#include "fin_date.h"

int main()
{
    // day count method - two dates
    {
        std::cout << "DAY COUNT METHOD - TWO DATES" << std::endl;

        // day count method
        std::string dcm;

        // year fraction variable
        double year_fraction;

        // create date objects
        lib_date::myDate date1 = lib_date::myDate("30/12/1979", "dd/mm/yyyy");
        lib_date::myDate date2 = lib_date::myDate("21/11/2021", "dd/mm/yyyy");

        // 30/360
        dcm = "30_360";
        year_fraction = fin_date::day_count_method(date1, date2, dcm);
        std::cout << "Year fraction between dates " + date1.get_date_str() + " and " + date2.get_date_str() +\
            " assuming " + dcm + " is " + std::to_string(year_fraction) + " years" << std::endl;

        // actual/360
        dcm = "ACT_360";
        year_fraction = fin_date::day_count_method(date1, date2, dcm);
        std::cout << "Year fraction between dates " + date1.get_date_str() + " and " + date2.get_date_str() +\
            " assuming " + dcm + " is " + std::to_string(year_fraction) + " years" << std::endl;

        // actual/365
        dcm = "ACT_365";
        year_fraction = fin_date::day_count_method(date1, date2, dcm);
        std::cout << "Year fraction between dates " + date1.get_date_str() + " and " + date2.get_date_str() +\
            " assuming " + dcm + " is " + std::to_string(year_fraction) + " years" << std::endl;

        // actual/actual
        dcm = "ACT_ACT";
        year_fraction = fin_date::day_count_method(date1, date2, dcm);
        std::cout << "Year fraction between dates " + date1.get_date_str() + " and " + date2.get_date_str() +\
            " assuming " + dcm + " is " + std::to_string(year_fraction) + " years" << std::endl;

        std::cout << '\n' << std::endl;
    }

    // day count method - vector of dates
    {
        std::cout << "DAY COUNT METHOD - VECTOR OF DATES" << std::endl;

        // day count method
        std::string dcm = "ACT_360";

        // dates
        lib_date::myDate date_ref = lib_date::myDate(20240101);

        std::vector<lib_date::myDate> dates_begin;
        dates_begin.clear();
        dates_begin.push_back(lib_date::myDate(20240201));
        dates_begin.push_back(lib_date::myDate(20240301));
        dates_begin.push_back(lib_date::myDate(20240401));

        std::vector<lib_date::myDate> dates_end;
        dates_end.clear();
        dates_end.push_back(lib_date::myDate(20240801));
        dates_end.push_back(lib_date::myDate(20240901));
        dates_end.push_back(lib_date::myDate(20241001));

        // year fractions
        std::vector<double> yr_fracs;

        // year fraction between dates in vector
        std::cout << "year fraction between dates in vector" << std::endl;
        yr_fracs.clear();
        yr_fracs = fin_date::day_count_method(dates_begin, dcm);
        for (std::size_t idx = 0; idx < yr_fracs.size(); idx++)
            std::cout << dates_begin[idx].get_date_str() << " - " << dates_begin[idx + 1].get_date_str() << " => " << std::to_string(yr_fracs[idx]) << std::endl;

        // year fraction between reference date and dates in vector
        std::cout << "year fraction between reference date and dates in vector" << std::endl;
        yr_fracs.clear();
        yr_fracs = fin_date::day_count_method(date_ref, dates_begin, dcm);
        for (std::size_t idx = 0; idx < yr_fracs.size(); idx++)
            std::cout << date_ref.get_date_str() << " - " << dates_begin[idx].get_date_str() << " => " << std::to_string(yr_fracs[idx]) << std::endl;

        // year fraction between two date vectors
        std::cout << "year fraction between two date vectors" << std::endl;
        yr_fracs.clear();
        yr_fracs = fin_date::day_count_method(dates_begin, dates_end, dcm);
        for (std::size_t idx = 0; idx < yr_fracs.size(); idx++)
            std::cout << dates_begin[idx].get_date_str() << " - " << dates_end[idx].get_date_str() << " => " << std::to_string(yr_fracs[idx]) << std::endl;

        std::cout << '\n' << std::endl;
    }

    // date rolling
    {
        std::cout << "DATE ROLLING" << std::endl;

        // define begining and end date
        lib_date::myDate date_begin = lib_date::myDate(20230523);
        lib_date::myDate date_end = date_begin;
        date_end.add("1Y");

        // get list of Czech holidays
        std::vector<lib_date::myDate> holidays = lib_date::get_holidays_cz(date_begin.get_year(), date_end.get_year());

        // generate a time series
        std::string freq = "1M";
        std::string date_format = "yyyymmdd";
        std::vector<lib_date::myDate> dates = lib_date::create_date_serie(date_begin.get_date_str(), date_end.get_date_str(), freq, date_format);

        // print the original dates
        std::cout << "Dates before rolling" << std::endl;
        for (unsigned short idx = 0; idx < dates.size(); idx++)
            std::cout << dates[idx].get_date_str() << std::endl;

        // roll dates
        std::string drm = "following";
        fin_date::date_rolling(dates, holidays, drm);

        // print the rolled dates
        std::cout << "Dates after rolling" << std::endl;
        for (unsigned short idx = 0; idx < dates.size(); idx++)
            std::cout << dates[idx].get_date_str() << std::endl;

        std::cout << '\n' << std::endl;
    }

    // financial date series
    {
        std::cout << "FINANCIAL DATE SERIE" << std::endl;

        std::string date_begin_str = "20220429";
        std::vector<std::string> date_freqs = {"ON", "TN", "1W", "2W", "1M", "2M", "1M-3M", "4M", "5M", "6M", "7M", "8M", "9M", "10M", "11M", "12M", "15M", "18M", "21M", "2Y", "3Y", "4Y", "5Y", "6Y", "7Y", "8Y", "9Y", "10Y", "11Y", "12Y", "13Y", "14Y", "15Y", "16Y", "17Y", "18Y", "19Y", "20Y", "21Y", "22Y", "23Y", "24Y", "25Y", "26Y", "27Y", "28Y", "29Y", "30Y"};
        std::string cnty = "de";
        std::string drm = "following";
        std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>> dates = fin_date::create_date_serie(date_begin_str, date_freqs, cnty, drm);

        for (unsigned short idx = 0; idx < std::get<0>(dates).size(); idx++)
            std::cout << std::get<0>(dates)[idx].get_date_str() << " " << std::get<1>(dates)[idx].get_date_str() << std::endl;

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}