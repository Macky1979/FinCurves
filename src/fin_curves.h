/**
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

#include <vector>
#include <map>
#include <memory>
#include "lib_date.h"

namespace fin_curves
{
    /**
     * @brief Convert discount factor into zero rate.depo
     * 
     * @param df Discount factor.
     * @param yr_frac Year fraction, which is used to convert discount factor into zero rate.
     * @param type Type of conversion. \n 0 => df = 1 / (1 + zr) ^ yr_frac \n 1 => df = exp(-zr * yr_frac)
     * @return double
     */
    double df_to_zr(const double& df, const double& yr_frac, const unsigned short type = 0);

    /**
     * @brief Convert discount factors into zero rates.
     * 
     * @param dfs Vector of discount rates.
     * @param yr_frac Vector of year fractions, which are used to convert discount factors into zero rates.
     * @param type Type of conversion. \n 0 => df = 1 / (1 + zr) ^ yr_frac \n 1 => df = exp(-zr * yr_frac)
     * @return std::vector<double>
     */
    std::vector<double> df_to_zr(const std::vector<double>& dfs, const std::vector<double>& yr_frac, const unsigned short type = 0);

	/**
     * @brief Depo generator.
     * 
     */
    struct depo_generator
    {
        std::string cal_pmt;
        std::string drm;
        std::string dcm;
        int days_shift;
    };

    /**
     * @brief FRA generator.
     * 
     */
    struct fra_generator
    {
        std::string cal_fix;
        std::string cal_pmt;
        std::string drm;
        std::string dcm;
        int days_shift;
        std::string period;
    };

    /**
     * @brief OIS generator.
     * 
     */
    struct ois_generator
    {
        std::string drm;
        std::string cal_fix;
        std::string cal_pmt;
        int days_shift;
        std::string fix_leg_dcm;
        std::string flt_leg_dcm;
        std::string simple_vs_compounding;
    };

    /**
     * @brief Interest rate swap generator.
     * 
     */
    struct irswp_generator
    {
        std::string drm;
        std::string fix_leg_cal_pmt;
        std::string fix_leg_dcm;
        std::string fix_leg_freq;
        int fix_leg_days_shift;
        std::string flt_leg_cal_fix;
        std::string flt_leg_cal_pmt;
        std::string flt_leg_dcm;
        std::string flt_leg_freq;
        int flt_leg_days_shift;
    };

    /**
     * @brief Basis swap generator.
     * 
     */
    struct bsswp_generator
    {
        std::string drm;
        std::string dom_leg_cal_fix;
        std::string dom_leg_cal_pmt;
        std::string dom_leg_dcm;
        std::string dom_leg_freq;
        int dom_leg_days_shift;
        std::string for_leg_cal_fix;
        std::string for_leg_cal_pmt;
        std::string for_leg_dcm;
        std::string for_leg_freq;
        int for_leg_days_shift;
    };

    /**
     * @brief General generator.
     * 
     */
    union generator
    {
        depo_generator depo;
        fra_generator fra;
        ois_generator ois;
        irswp_generator irswp;
        bsswp_generator bsswp;
        generator(){};
        ~generator(){};
    };
    
    /**
     * @brief Object holding curve generators' definitions.
     * 
     */
    class myGenerators
    {
        private:

        public:

            /**
             * @brief Map holding curve generators' definitions.
             * 
             */
            std::map<std::string, std::shared_ptr<generator>> instruments;

            /**
             * @brief Construct a new myGenerators object.
             * 
             * @param instrument_nms Vector of generators' names.
             */
            myGenerators(const std::vector<std::string>& instrument_nms);

            /**
             * @brief Return generator for a specified instrument.
             * 
             * @param instrument_nm Name of instrument for which the generator is to be returned.
             * @return generator Generator's definition.
             */
            generator get(const std::string& instrument_nm) const;
    };

    /**
     * @brief Curve quote.
     * 
     */
    struct crv_quote
    {
        std::string tenor;
        lib_date::myDate date;
        double yr_fraction;
        std::string generator;
        double quote;
    };
}