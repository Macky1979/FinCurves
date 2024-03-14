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
    struct zr_interp_def
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
            std::string date;
            bool quotes = false;
            std::vector<double> yr_fracs;
            std::vector<std::shared_ptr<double>> yr_fracs_aux;
            std::vector<double> zrs;
            std::vector<std::shared_ptr<double>> zrs_aux;
            fin_curves::zr_interp_def interp_def;

        public:
            /**
             * @brief Construct a new myZeroRate object from definition of curve interpolation.
             * 
             * @param crv_nm Curve name.
             * @param interp_def Structure describing curve interpolation.
             * @param path Path where curve definition and zero rates should be stored.
             */
            myZeroRate(const std::string& crv_nm, const fin_curves::zr_interp_def& interp_def, const std::string& path = "--na--");

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

             * @param date Date of the provided zero rates.
             * @param yr_fracs Vector of maturity year fractions.
             * @param zrs Vector of zero rates correspoding to maturity year fractions.
             * @param date_format Format of the date.
             */
            void load(const std::string& date, const std::vector<double>& yr_fracs, const std::vector<double>& zrs, const std::string& date_format = "yyyymmdd");

            /**
             * @brief Load year fractions and corresponding zero rates from a .csv file.
             * 
             * @param date Date of the .csv file. This is also supposed to be date of the zero rates stored in the file.
             * @param sep Column separator.
             * @param date_format Format of the date.
             * @param quotes true => Strings are enclosed in quotes. \n false => Strings are not enclosed in quotes.
             */
            void load(const std::string& date, const std::string& date_format = "yyyymmdd", const std::string& sep = ",", const bool& quotes = false);

            /**
             * @brief Get curve name.
             * 
             * @return std::string Curve name.
             */
            std::string get_crv_nm(){return this->crv_nm;};

            /**
             * @brief Get the zero rate using year fraction.
             * 
             * @param yr_frac Year fraction.
             * @return double Zero rate.
             */
            double get_zr(const double& yr_frac) const;

            /**
             * @brief Get the zero rate using index.
             * 
             * @param yr_frac Zero rate position index starting with 0.
             * @return double Zero rate.
             */
            double get_zr(const std::size_t& idx) const {return this->zrs[idx];};

            /**
             * @brief Get the discount factor using year fraction.
             * + this->crv_nm
             * @param yr_frac Year fraction.
             * @return double Zero rate.
             */
            double get_df(const double& yr_frac) const;

            /**
             * @brief Get the discount factor using index.
             * 
             * @param yr_frac Discount factor position index starting with 0.
             * @return double Discount factor.
             */
            double get_df(const std::size_t& idx) const {return fin_curves::zr_to_df(this->zrs[idx], this->yr_fracs[idx], this->interp_def.zr_type);};

            /**
             * @brief Get full vector of year_fractions.
             * 
             * @return std::vector<double> Vector of year fractions.
             */
            std::vector<double> get_yr_fracs() const {return this->yr_fracs;};

            /**
             * @brief Add tuple containing year fraction and zero rate to the object.
             * 
             * @param yr_frac_zr Tuple containing year fraction and zero rate.
             */
            void add(const std::tuple<double, double>& yr_frac_zr) {this->yr_fracs.push_back(std::get<0>(yr_frac_zr)); this->zrs.push_back(std::get<1>(yr_frac_zr));};

            /**
             * @brief Get the interpolation definition.
             * 
             * @return fin_curves::interp_def Interpolation definition.
             */
            fin_curves::zr_interp_def get_def(){return this->interp_def;};

            /**
             * @brief Reset vector of year fractions and zero rates.
             * 
             */
            void reset();

            /**
             * @brief Save zero curve interpolation definition into a .csv file.
             * 
             * sep Separator to be used in the .csv file.
             */
            void save_def(const std::string& sep = ",");

            /**
             * @brief Save zero curve data into a .csv file.
             * 
             * @param sep Separator to be used in the .csv file.
             */
            void save_data(const std::string& sep = ",");
    };
}