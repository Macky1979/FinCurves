#include <cmath>
#include <vector>
#include <stdexcept>
#include "lib_math.h"
#include "lib_tbl.h"
#include "fin_date.h"
#include "fin_curves.h"

double fin_curves::zr_to_df(const double& zr, const double& yr_frac, const unsigned short type)
{
    // df = 1 / (1 + zr) ^ yr_frac
    if (type == 0)
    {
        return 1.0 / std::pow(1 + zr, yr_frac); 
    }
    // df = exp(-zr * yr_frac)
    else if (type == 1)
    {
        return std::exp(-zr * yr_frac);
    }
    // unsupported conversion type
    else
        throw std::runtime_error((std::string)__func__ + ": '" + std::to_string(type) + "' is not a supported conversion type!");
}

std::vector<double> fin_curves::zr_to_df(const std::vector<double>& zrs, const std::vector<double>& yr_fracs, const unsigned short type)
{
    // vector of zero rates
    std::vector<double> dfs;

    // go zero rate by zero rate
    for (std::size_t idx = 0; idx < zrs.size(); idx++)
    {
        dfs.push_back(df_to_zr(zrs[idx], yr_fracs[idx], type));
    }

    // return vector of zero rates
    return dfs;
}

double fin_curves::df_to_zr(const double& df, const double& yr_frac, const unsigned short type)
{
    // df = 1 / (1 + zr) ^ yr_frac
    if (type == 0)
    {
        return std::pow(df, -1.0 / yr_frac) - 1.0;
    }
    // df = exp(-zr * yr_frac)
    else if (type == 1)
    {
        return -1.0 * std::log(df) / yr_frac;
    }
    // unsupported conversion type
    else
        throw std::runtime_error((std::string)__func__ + ": '" + std::to_string(type) + "' is not a supported conversion type!");
}

std::vector<double> fin_curves::df_to_zr(const std::vector<double>& dfs, const std::vector<double>& yr_fracs, const unsigned short type)
{
    // vector of zero rates
    std::vector<double> zrs;

    // go discount factor by discount factor
    for (std::size_t idx = 0; idx < dfs.size(); idx++)
    {
        zrs.push_back(df_to_zr(dfs[idx], yr_fracs[idx], type));
    }

    // return vector of zero rates
    return zrs;
}

std::tuple<std::vector<double>, std::vector<double>> fin_curves::interp_rate(const std::vector<std::shared_ptr<double>>& yr_fracs, const std::vector<std::shared_ptr<double>>& rates, const std::string& interp_tp)
{
    // create vectors to hold year fractions and rates after dropping missing rates
    std::vector<double> yr_fracs_no_missing;
    std::vector<double> rates_no_missing;

    // create vectors to hold year fractions and rates after interpolation
    std::vector<double> yr_fracs_interp;
    std::vector<double> rates_interp;

    // go through rates and drop missing values identified through a null pointer
    for (std::size_t idx = 0; idx < rates.size(); idx++)
    {
        if (rates[idx] != nullptr)
        {
            yr_fracs_no_missing.push_back(*yr_fracs[idx]);
            rates_no_missing.push_back(*rates[idx]);
        }
    }

    // go through rates and interpolate missing values identified through a null pointer
    for (std::size_t idx = 0; idx < rates.size(); idx++)
    {
        // rate is not missing => take over both year fraction and corresponding rate
        if (rates[idx] != nullptr)
        {
            yr_fracs_interp.push_back(*yr_fracs[idx]);
            rates_interp.push_back(*rates[idx]);
        }
        // rate is missing => interpolate the missing value
        else
        {
            yr_fracs_interp.push_back(*yr_fracs[idx]);
            double rate;
            if (interp_tp.compare("linear") == 0)
                rate = lib_math::lin_interp1d(yr_fracs_no_missing, rates_no_missing, std::vector<double>(*yr_fracs[idx]), false)[0]; // false => we do not extrapolate
            else
                throw std::runtime_error((std::string)__func__ + ": '" + interp_tp + "' is not a supported interpolation!");
            rates_interp.push_back(rate);
        }
    }

    // return vector of year fractions and vector of rates with interpolated missing values
    return std::tuple<std::vector<double>, std::vector<double>>(yr_fracs_interp, rates_interp);
}