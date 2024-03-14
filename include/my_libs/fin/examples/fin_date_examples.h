#include <string>
#include <algorithm>
#include "lib_math.h"
#include "fin_date.h"

// decompose frequncy string in frequency amount and frequency unit
std::tuple<int, std::string> fin_date::decompose_freq(const std::string& freq_str)
{
    // special treatment of O/N and T/N tenor
    if (freq_str.compare("ON") == 0)
    {
        std::tuple<int, std::string> freq = {-2, "D"};
        return freq;
    }
    else if (freq_str.compare("TN") == 0)
    {
        std::tuple<int, std::string> freq = {-1, "D"};
        return freq;
    }

    // suported frequency units
    std::vector<std::string> freq_units = {"D", "W", "M", "Y"};

    // determine if the frequency string is e.g. "1Y" or "1Y3M"
    std::vector<std::string> freq_units_sep;
    for (std::size_t idx = 0; idx < freq_str.size(); idx++)
    {
        if (std::find(freq_units.begin(), freq_units.end(), freq_str.substr(idx, 1)) != freq_units.end())
            freq_units_sep.push_back(freq_str.substr(idx, 1));
    }

    // check frequency units in the string
    if (freq_units_sep.size() == 0)
        throw std::invalid_argument((std::string)__func__ + ": No supported frequency unit found in '" + freq_str + "'!");
    else if (freq_units_sep.size() > 2)
        throw std::invalid_argument((std::string)__func__ + ": More than two frequency units found in '" + freq_str + "'!");

    // check that frequency units have a correct order
    if (freq_units_sep.size() == 2)
    {
        if ((freq_units_sep[0].compare("Y") == 1) or (freq_units_sep[1].compare("M") == 1))
            throw std::invalid_argument((std::string)__func__ + ": '" + freq_str + "' is not a supported frequency string!");
    }

    // define variable holding frequency amount and frequency unit
    int freq_amt;
    std::string freq_unit;

    // decompose frequency string
    if (freq_units_sep.size() == 1)
    {
        // split frequency string to amount and unit
        freq_amt = std::stoi(freq_str.substr(0, freq_str.size() - 1));
        freq_unit = freq_str.substr(freq_str.size() - 1, 1);

        // convert years to months
        if (freq_unit.compare("Y") == 0)
        {
            freq_unit = "M";
            freq_amt *= 12;
        }
    }
    else
    {
        std::size_t pos;

        pos = freq_str.find(freq_units_sep[0]);
        int freq_amt_yr = std::stoi(freq_str.substr(pos - 1, 1));

        pos = freq_str.find(freq_units_sep[1]);
        int freq_amt_mth = std::stoi(freq_str.substr(pos - 1, 1));

        freq_amt = freq_amt_yr * 12 + freq_amt_mth;
        freq_unit = "M";
    }

    // return frequency amout and frequency unit
    std::tuple<int, std::string> freq = {freq_amt, freq_unit};
    return freq;
}

// compose frequency string from frequency amount and frequency unit
std::string fin_date::compose_freq(const std::tuple<int, std::string>& freq_decomposed)
{
    // specidal treatment for "ON"
    if ((std::get<0>(freq_decomposed) == -2) && (std::get<1>(freq_decomposed).compare("D") == 0))
        return "ON";
    // special treatment for "TN"
    else if ((std::get<0>(freq_decomposed) == -1) && (std::get<1>(freq_decomposed).compare("D") == 0))
        return "TN";
    // other case
    else
    {
        // suported frequency units
        std::vector<std::string> freq_units = {"D", "W", "M", "Y"};

        // compose the frequency amount and frequency unit
        if (std::find(freq_units.begin(), freq_units.end(), std::get<1>(freq_decomposed)) != freq_units.end())
            return std::to_string(std::get<0>(freq_decomposed)) + std::get<1>(freq_decomposed);
        else
            throw std::invalid_argument((std::string)__func__ + ": '" + std::get<1>(freq_decomposed) + "' is not a supported frequency unit!");
    }
}

// convert frequency amount and frequency unit, e.g. [1, "M"], into proxy maturity, e.g. 1 / 12 = 0.0833333.
double fin_date::get_tenor_maturity(const std::tuple<int, std::string>& freq_decomposed)
{
    if (std::get<1>(freq_decomposed).compare("D") == 0)
        return std::get<0>(freq_decomposed) / 365.0;
    else if (std::get<1>(freq_decomposed).compare("W") == 0)
        return std::get<0>(freq_decomposed) / (365.0 / 7.0);
    else if (std::get<1>(freq_decomposed).compare("M") == 0)
        return std::get<0>(freq_decomposed) / 12.0;
    else if (std::get<1>(freq_decomposed).compare("Y") == 0)
        return std::get<0>(freq_decomposed) / 1.0;
    else
        throw std::invalid_argument((std::string)__func__ + ": '" + std::get<1>(freq_decomposed) + "' is not a supported frequency unit!");
}

// create a list of frequency amounts and units, e.g. [[1, "M"], [2, "M"], [3, "M"], ...], till maturity specified in years.
std::vector<std::tuple<int, std::string>> fin_date::generate_freqs(const std::tuple<int, std::string>& freq_decomposed, const unsigned short& maturity)
{
    // check frequency init
    if ((std::get<1>(freq_decomposed).compare("M") == 1) && (std::get<1>(freq_decomposed).compare("Y")))
        throw std::invalid_argument((std::string)__func__ + ": '" + std::get<1>(freq_decomposed) + "' is not a supported frequency unit!");

    // list of decomposed frequencies
    std::vector<std::tuple<int, std::string>> freqs;

    // generate frequency series
    int freq_amt = std::get<0>(freq_decomposed);
    std::string freq_unit = std::get<1>(freq_decomposed);
    double actual_maturity = fin_date::get_tenor_maturity(freq_decomposed);

    while (actual_maturity <= maturity)
    {
        freqs.push_back(std::tuple<int, std::string>(freq_amt, freq_unit));
        freq_amt++;
        actual_maturity = fin_date::get_tenor_maturity(std::tuple<int, std::string>(freq_amt, freq_unit));
    }

    // return list of decomposed frequencies
    return freqs;
}

// drop duplicates from a frequency list; the list has a form of [[1, "M"], [2, "M"], [3, "M"], ...].
std::vector<std::tuple<int, std::string>> fin_date::drop_freq_duplicates(const std::vector<std::tuple<int, std::string>>& freqs_decomposed)
{
    std::vector<std::tuple<int, std::string>> freqs_no_duplicities;

    for (std::size_t idx = 0; idx < freqs_decomposed.size(); idx++)
    {
        if (not (std::find(freqs_no_duplicities.begin(), freqs_no_duplicities.end(), freqs_decomposed[idx]) != freqs_no_duplicities.end()))
            freqs_no_duplicities.push_back(freqs_decomposed[idx]);
    }

    return freqs_no_duplicities;
}

// sort list of frequency amounts and units in a form of [[1, "M"], [2, "M"], [3, "M"], ...]
std::vector<std::tuple<int, std::string>> fin_date::sort_freqs(const std::vector<std::tuple<int, std::string>>& freqs_decomposed)
{
    // determine tenor maturities
    std::vector<double> maturities;
    for (std::size_t idx = 0; idx < freqs_decomposed.size(); idx++)
        maturities.push_back(fin_date::get_tenor_maturity(freqs_decomposed[idx]));

    // sort vector by index
    std::vector<std::size_t> i = lib_math::sort_index(maturities);

    // sort frequencies using sorted indicies
    std::vector<std::tuple<int, std::string>> freqs_sorted;
    for (std::size_t idx = 0; idx < i.size(); idx++)
        freqs_sorted.push_back(freqs_decomposed[i[idx]]);

    // return sorted frequency vector
    return freqs_sorted;
}

// add or subtract two decomposed frequencies provided they have the same frequecy unit, e.g. [1, "M"] and [2, "M"]
std::tuple<int, std::string> fin_date::combine_freqs(const std::tuple<int, std::string>& freq1, const std::tuple<int, std::string>& freq2, const std::string& operand)
{
    // check if the two decomposed frequencies have the same unit
    if (std::get<1>(freq1) != std::get<1>(freq2))
        throw std::invalid_argument((std::string)__func__ + ": The two frequencies must have the same frequency unit!");

    // extract frequency amounts
    int freq1_amt = std::get<0>(freq1);
    int freq2_amt = std::get<0>(freq2);

    // combine the two decomposed frequencies
    if (operand == "+")
        return std::tuple<int, std::string>(freq1_amt + freq2_amt, std::get<1>(freq1));
    else if (operand == "-")
        return std::tuple<int, std::string>(freq1_amt - freq2_amt, std::get<1>(freq1));
    else
        throw std::invalid_argument((std::string)__func__ + ": '" + operand + "' is not a supported operand!");
}

// calculate year fractions for family of 30/360 methods
double calc_year_fraction(const int& day1, const int& month1, const int& year1, const int& day2, const int& month2, const int& year2)
{
        // calculate year fraction
        return (360. * (year2 - year1) + 30. * (month2 - month1) + (day2 - day1)) / 360.;
}

// implementation of day count method; see https://en.wikipedia.org/wiki/Day_count_convention
double fin_date::day_count_method(const lib_date::myDate& date_begin, const lib_date::myDate& date_end, const std::string& dcm)
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

    // check that beging date preceeds end date
    if (date_begin.get_days_no() >= date_end.get_days_no())
    {
        throw std::invalid_argument((std::string)__func__ + ": Parameter date_begin must preceed date_end!");
    }

    // extract day, month and year from begining date
    day1 = date_begin.get_day();
    month1 = date_begin.get_month();
    year1 = date_begin.get_year();

    // extract day, month and year from end date
    day2 = date_end.get_day();
    month2 = date_end.get_month();
    year2 = date_end.get_year();

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
        if ((month1 == 2) and date_begin.is_last_day_in_month())
        {
            last_february_day1 = true;
        }
        else
        {
            last_february_day1 = false;
        }

        // determine if date2 is the last February day
        if ((month2 == 2) and date_end.is_last_day_in_month())
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
        year_fraction = (date_end.get_days_no() - date_begin.get_days_no()) / 360.;
    }
    // actual / 365 method
    else if (dcm.compare("ACT_365") == 0)
    {
        // calculate year fraction
        year_fraction = (date_end.get_days_no() - date_begin.get_days_no()) / 365.;
    }
    // actual / actual ISDA method
    else if (dcm.compare("ACT_ACT") == 0)
    {
        // date1 and date2 fall into the same year
        if (year1 == year2)
        {
            // the year is a leap year
            if (date_begin.is_leap_year())
            {
                // calculate year fraction
                year_fraction = (date_end.get_days_no() - date_begin.get_days_no()) / 366.;
            }
            else
            {
                // calculate year fraction
                year_fraction = (date_end.get_days_no() - date_begin.get_days_no()) / 365.; 
            }
        }
        // date1 and date2 fall into different years
        else
        {   
            // auxiliary variables
            double year_fraction1_aux;
            double year_fraction2_aux;

            // create auxiliary dates
            lib_date::myDate date1_aux = lib_date::myDate(date_begin.get_year() * 10000 + 1231);
            lib_date::myDate date2_aux = lib_date::myDate(date_end.get_year() * 10000 + 101);

            // calculate the first auxiliary year fraction
            year_fraction1_aux = date1_aux.get_days_no() - date_begin.get_days_no();
            if (date_begin.is_leap_year())
            {
                year_fraction1_aux /= 366;
            }
            else
            {
                year_fraction1_aux /= 365;
            }

            // calculate the seconds auxiliary year fraction
            year_fraction2_aux = date_end.get_days_no() - date2_aux.get_days_no();
            if (date_end.is_leap_year())
            {
                year_fraction2_aux /= 366;
            }
            else
            {
                year_fraction2_aux /= 365;
            }

            // calculate the final year fraction
            year_fraction = year_fraction1_aux + year_fraction2_aux + (date_end.get_year() - date_begin.get_year() - 1);
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

std::vector<double> fin_date::day_count_method(const std::vector<lib_date::myDate>& dates, const std::string& dcm)
{
    // vector with year fractions
    std::vector<double> yr_fracs;

    // go through vector of dates and calculate year fraction between two neighboring dates
    for (std::size_t idx = 0; idx < dates.size() - 1; idx++)
    {
        double yr_frac = fin_date::day_count_method(dates[idx], dates[idx + 1], dcm);
        yr_fracs.push_back(yr_frac);
    }

    // return calculated year fractions
    return yr_fracs;
}

std::vector<double> fin_date::day_count_method(const lib_date::myDate& date_ref, const std::vector<lib_date::myDate>& dates, const std::string& dcm)
{
    // vector with year fractions
    std::vector<double> yr_fracs;

    // go through vector of dates and calculate year fraction between the dates in vector and reference date.
    for (std::size_t idx = 0; idx < dates.size(); idx++)
    {
        double yr_frac = fin_date::day_count_method(date_ref, dates[idx], dcm);
        yr_fracs.push_back(yr_frac);
    }

    // return calculated year fractions
    return yr_fracs;
}

std::vector<double> fin_date::day_count_method(const std::vector<lib_date::myDate>& dates_begin, const std::vector<lib_date::myDate>& dates_end, const std::string& dcm)
{
    // check that the two date vectors are of the same length
    if (dates_begin.size() != dates_end.size())
        throw std::invalid_argument((std::string)__func__ + ": Vectors dates_begin and dates_end must be of the same length!");

    // vector with year fractions
    std::vector<double> yr_fracs;

    // go through the two vectors and calculate year fraction between their corresponding dates.
    for (std::size_t idx = 0; idx < dates_begin.size(); idx++)
    {
        double yr_frac = fin_date::day_count_method(dates_begin[idx], dates_end[idx], dcm);
        yr_fracs.push_back(yr_frac);
    }

    // return calculated year fractions
    return yr_fracs;
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
std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>> fin_date::create_date_serie(const std::string& date_start_str, const std::vector<std::string>& date_freqs, const std::string& cnty, const std::string& drm, const std::string& date_format)
{
    // auxiliary dates
    lib_date::myDate date_start = lib_date::myDate(date_start_str, date_format);
    lib_date::myDate date_ref = lib_date::myDate(date_start_str, date_format);

    // determine holidays calendar
    std::size_t year_start = date_start.get_year();
    std::size_t year_end = year_start + 100;
    std::vector<lib_date::myDate> holidays;

    if (cnty.compare("cz") == 0)
        holidays = lib_date::get_holidays_cz(year_start, year_end);
    else if (cnty.compare("de") == 0)
        holidays = lib_date::get_holidays_de(year_start, year_end);
    else if (cnty.compare("uk") == 0)
        holidays = lib_date::get_holidays_uk(year_start, year_end);
    else if (cnty.compare("us") == 0)
        holidays = lib_date::get_holidays_us(year_start, year_end);
    else
        holidays = lib_date::get_weekends(year_start, year_end);

    // go through date frequencies
    std::vector<lib_date::myDate> dates_begin;
    std::vector<lib_date::myDate> dates_end;
    for (unsigned short idx = 0; idx < date_freqs.size(); idx++)
    {
        // process O/N
        if (date_freqs[idx].compare("ON") == 0)
        {
            lib_date::myDate date_begin = lib_date::myDate(date_start.get_date_int());
            dates_begin.push_back(date_begin);

            lib_date::myDate date_end = lib_date::myDate(date_start.get_date_int());
            date_end.add_working_days(holidays, 1);
            dates_end.push_back(date_end);

            date_ref.set(date_end.get_date_int());

        // process T/N
        }
        else if (date_freqs[idx].compare("TN") == 0)
        {
            lib_date::myDate date_begin = lib_date::myDate(date_start.get_date_int());
            date_begin.add_working_days(holidays, 1);
            dates_begin.push_back(date_begin);

            lib_date::myDate date_end = lib_date::myDate(date_start.get_date_int());
            date_end.add_working_days(holidays, 2);
            dates_end.push_back(date_end);

            date_ref.set(date_end.get_date_int());
        }

        // process other date frequency strings
        else
        {
            // check for frequency string like 1M-3M
            if (date_freqs[idx].find("-") != std::string::npos)
            {
                std::size_t pos = date_freqs[idx].find("-");

                std::string date_freq_1 = date_freqs[idx].substr(0, pos);
                lib_date::myDate date_begin = lib_date::myDate(date_ref.get_date_int());
                date_begin.add(date_freq_1);
                dates_begin.push_back(date_begin);

                std::string date_freq_2 = date_freqs[idx].substr(pos + 1);
                lib_date::myDate date_end = lib_date::myDate(date_ref.get_date_int());
                date_end.add(date_freq_2);
                dates_end.push_back(date_end);
            }

            // check for frequency string like 1M
            else
            {
                dates_begin.push_back(date_ref);

                lib_date::myDate date_end = lib_date::myDate(date_ref.get_date_int());
                date_end.add(date_freqs[idx]);
                dates_end.push_back(date_end);
            }
        }
    }

    // roll end dates; the begining dates should be OK as they are addjusted for working days in ON and TN part of the code
    for (unsigned short idx = 0; idx < dates_end.size(); idx++)
        date_rolling(dates_end, holidays, drm);

    // return date serie
    return std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>>({dates_begin, dates_end});

}