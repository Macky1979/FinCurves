/** \example fin_zerorate_examples.h
 * @file fin_zerorate.h
 * @author Michal Mackanic
 * @brief Zerorate object returns zero rates / discounting factors.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <iostream>
#include <vector>
#include "fin_curves.h"

namespace fin_curves
{
    /**
     * @brief Structure describing zero rate curve interpolation.
     * 
     */
    struct interp_def
    {
        std::string ccy;
        std::string zr_dcm;
        std::string zr_interp;
        int zr_type;
    };

    /**
     * @brief ZeroRate object.
     * 
     */
    class myZeroRate
    {
        private:
            std::string path = "--na--";
            std::string crv_nm = "--na--";
            std::string sep = ",";
            bool quotes = false;
            std::vector<double> yr_fracs;
            std::vector<std::shared_ptr<double>> yr_fracs_aux;
            std::vector<double> zrs;
            std::vector<std::shared_ptr<double>> zrs_aux;
            fin_curves::interp_def interp_def;

        public:
            /**
             * @brief Construct a new myZeroRate object from definition of curve interpolation.
             * 
             * @param interp_def Structure describing curve interpolation.
             */
            myZeroRate(const fin_curves::interp_def& interp_def);

            /**
             * @brief Construct a new myZeroRate object using information in definition file.
             * 
             * @param path Path with curve interpolation definition.
             * @param crv_nm Curve name that corresponds to .csv file name with curve interpolation defition.
             * @param sep Column separator.
             * @param quotes true => Strings are enclosed in quotes. \n false => Strings are not enclosed in quotes.
             */
            myZeroRate(const std::string& path, const std::string& crv_nm, const std::string& sep = ",", const bool& quotes = false);

            /**
             * @brief Load year fractions and corresponding zero rates.
             * 
             * @param yr_fracs Vector of year fractions.
             * @param zrs  Vector of zero rates.
             * @param interp_tp Interpolation type to be applied on potentially missing rates; currently the only supported type is linear interpolation.
             */
            void load(const std::vector<double>& yr_fracs, const std::vector<double>& zrs);

            /**
             * @brief Load year fractions and corresponding zero rates from a .csv file.
             * 
             * @param date Date of the .csv file.
             * @param sep Column separator.
             * @param quotes true => Strings are enclosed in quotes. \n false => Strings are not enclosed in quotes.
             * @param date_format Format of the date.
             * @param interp_tp Interpolation type to be applied on potentially missing rates; currently the only supported type is linear interpolation.
             */
            void load(const std::string& date, const std::string& date_format = "yyyymmdd");

            /**
             * @brief Get curve name.
             * 
             * @return std::string Curve name.
             */
            std::string get_crv_nm(){return this->crv_nm;};

            /**
             * @brief Get the zero rate.
             * 
             * @param yr_frac Year fraction.
             * @return double Zero rate.
             */
            double get_zr(const double& yr_frac) const;

            /**
             * @brief Get the discount factor.
             * 
             * @param yr_frac Year fraction.
             * @return double Zero rate.
             */
            double get_df(const double& yr_frac) const;

            /**
             * @brief Get the interpolation definition.
             * 
             * @return fin_curves::interp_def Interpolation definition.
             */
            fin_curves::interp_def get_def(){return this->interp_def;};

            /**
             * @brief Reset vector of year fractions and zero rates.
             * 
             */
            void reset();
    };
}