/** \example fin_curves_examples.h
 * @file fin_curves.h
 * @author Michal Mackanic
 * @brief Implement financial curve boostrapping.
 * @version 1.0
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
#include <iostream>
#include "fin_curves.h"

int main()
{
    // test df_to_zr() functions
    {
        std::cout << "TEST df_to_zr() FUNCTIONS" << std::endl;

        std::cout << "double df_to_zr(const double& df, const double& yr_frac, const unsigned short type = 0)" << std::endl;
        double df = 0.999;
        double yr_frac = 1.0;
        double zr_1 = fin_curves::df_to_zr(df, yr_frac);
        double zr_2 = fin_curves::df_to_zr(df, yr_frac, 1);

        std::cout << "   df = 1 / (1 + zr) ^ yr_frac" << std::endl;
        std::cout << "      df = " << std::to_string(df) << " => zr = " << std::to_string(zr_1) << std::endl;

        std::cout << "   df = exp(-zr * yr_frac)" << std::endl;
        std::cout << "      df = " << std::to_string(df) << " => zr = " << std::to_string(zr_2) << std::endl;

        std::cout << "std::vector<double> df_to_zr(const std::vector<double>& dfs, const std::vector<double>& yr_frac, const unsigned short type = 0)" << std::endl;
        std::vector<double> dfs = {0.999, 1.001};
        std::vector<double> yr_fracs = {1.0, 1.0};
        std::vector<double> zrs_1 = fin_curves::df_to_zr(dfs, yr_fracs);
        std::vector<double> zrs_2 = fin_curves::df_to_zr(dfs, yr_fracs, 1);

        std::cout << "   df = 1 / (1 + zr) ^ yr_frac" << std::endl;
        for (std::size_t idx = 0; idx < dfs.size(); idx++)
            std::cout << "      df = " << std::to_string(dfs[idx]) << " => zr = " << std::to_string(zrs_1[idx]) << std::endl;

        std::cout << "   df = exp(-zr * yr_frac)" << std::endl;
        for (std::size_t idx = 0; idx < dfs.size(); idx++)
            std::cout << "      df = " << std::to_string(dfs[idx]) << " => zr = " << std::to_string(zrs_2[idx]) << std::endl;

        std::cout << '\n' << std::endl;
    }

    // test min_interp_points() function
    {
        std::cout << "TEST in_interp_points() FUNCTION" << std::endl;

        std::cout << "mininum number of points for linear interpolation: " << std::to_string(fin_curves::min_interp_points("linear")) << std::endl;
        std::cout << "mininum number of points for cubic interpolation: " << std::to_string(fin_curves::min_interp_points("cubic")) << std::endl;

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}
*/

# pragma once

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "lib_date.h"
#include "fin_generators.h"

namespace fin_curves
{
    /**
     * @brief Convert zero rate into discount factor
     * 
     * @param zr Zero rate.
     * @param yr_frac Year fraction, which is used to convert zero rate into discount factor.
     * @param type Type of conversion. \n 0 => df = 1 / (1 + zr) ^ yr_frac \n 1 => df = exp(-zr * yr_frac)
     * @return double Discount factor.
     */
    double zr_to_df(const double& zr, const double& yr_frac, const unsigned short type = 0);

    /**
     * @brief Convert zero rates into discount factors.
     * 
     * @param zrs Vector of zero rates.
     * @param yr_frac Vector of year fractions, which are used to convert zero rates into discount factors.
     * @param type Type of conversion. \n 0 => df = 1 / (1 + zr) ^ yr_frac \n 1 => df = exp(-zr * yr_frac)
     * @return std::vector<double> Vector of discount factors.
     */
    std::vector<double> zr_to_df(const std::vector<double>& zrs, const std::vector<double>& yr_frac, const unsigned short type = 0);

    /**
     * @brief Convert discount factor into zero rate.
     * 
     * @param df Discount factor.
     * @param yr_frac Year fraction, which is used to convert discount factor into zero rate.
     * @param type Type of conversion. \n 0 => df = 1 / (1 + zr) ^ yr_frac \n 1 => df = exp(-zr * yr_frac)
     * @return double Zero rate.
     */
    double df_to_zr(const double& df, const double& yr_frac, const unsigned short type = 0);

    /**
     * @brief Convert discount factors into zero rates.
     * 
     * @param dfs Vector of discount rates.
     * @param yr_frac Vector of year fractions, which are used to convert discount factors into zero rates.
     * @param type Type of conversion. \n 0 => df = 1 / (1 + zr) ^ yr_frac \n 1 => df = exp(-zr * yr_frac)
     * @return std::vector<double> Vector of zero rates.
     */
    std::vector<double> df_to_zr(const std::vector<double>& dfs, const std::vector<double>& yr_frac, const unsigned short type = 0);

    /**
     * @brief Interpolate missing rates. Missing rate is identified through a null pointer.
     * 
     * @param yr_fracs Vector of year fractions.
     * @param zrs Vector of zero rates.
     * @param interp_tp Interpolation type; currently the only supported type is linear interpolation.
     * @return std::tuple<double, double> Vector of year fractions and zero rates with missing zero rates being interpolated.
     */
    std::tuple<std::vector<double>, std::vector<double>> interp_rate(const std::vector<std::shared_ptr<double>>& yr_fracs, const std::vector<std::shared_ptr<double>>& zrs, const std::string& interp_tp = "linear");

    /**
     * @brief Object holding depo instrument.
     * 
     */
    class myDepoInstr
    {
        private:
            std::string date_exec;
            std::string tenor;
            std::vector<std::string> tenors;
            std::vector<std::tuple<lib_date::myDate, lib_date::myDate>> date_series;
            double quote;
            std::shared_ptr<fin_curves::generator> generator;
            std::tuple<lib_date::myDate, lib_date::myDate> dates_int;
            std::string crv_nm;

        public:
            myDepoInstr(std::string date_exec, std::string tenor, double quote, std::string crv_nm, std::shared_ptr<fin_curves::generator> generator, std::string date_format = "yyyymmdd");
            std::vector<std::tuple<lib_date::myDate, lib_date::myDate>> get_date_series(){return this->date_series;};
    };
}