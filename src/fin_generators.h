/** \example fin_generators_examples.h
 * @file fin_generators.h
 * @author Michal Mackanic
 * @brief Generator is a curve building block defining its individual underlying instruments.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <map>

namespace fin_curves
{
    /**
     * @brief Depo instrument.
     * 
     */
    struct depo
    {
        std::string cal_pmt;
        std::string drm;
        std::string dcm;
        int days_shift;
    };

    /**
     * @brief FRA instrument.
     * 
     */
    struct fra
    {
        std::string cal_fix;
        std::string cal_pmt;
        std::string drm;
        std::string dcm;
        int days_shift;
        std::string period;
    };

    /**
     * @brief OIS instrument.
     * 
     */
    struct ois
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
     * @brief Interest rate swap.
     * 
     */
    struct irswp
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
     * @brief Basis swap.
     * 
     */
    struct bsswp
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
     * @brief General financial instrument.
     * 
     */
    union instrument
    {
        fin_curves::depo depo;
        fin_curves::fra fra;
        fin_curves::ois ois;
        fin_curves::irswp irswp;
        fin_curves::bsswp bsswp;
        instrument(){};
        ~instrument(){};
    };

    /**
     * @brief Generator of financial instrument.
     * 
     */
    struct generator
    {
        std::string instr_nm;
        std::string instr_tp;
        fin_curves::instrument instr_def;
    };

    /**
     * @brief Object holding curve generators definitions.
     * 
     */
    class myGenerators
    {
        private:

        public:

            /**
             * @brief Map holding curve generators definitions.
             * 
             */
            std::map<std::string, std::shared_ptr<generator>> instruments;

            /**
             * @brief Construct a new myGenerators object using their definition stored in .csv files. Generators hold definitions needed for intrument evaluation, e.g. currency, day-count method, day-rolling method, fixing frequency, calendar, etc.
             * 
             * @param instrument_nms Vector of instrument names.
             * @param sep Column separator.
             * @param quotes true => Strings are enclosed in quotes. \n false => Strings are not enclosed in quotes.
             */
            myGenerators(const std::vector<std::string>& instrument_nms, const std::string& sep = ",", const bool& quotes = false);

            /**
             * @brief Return generator for a specified instrument.
             * 
             * @param instrument_nm Name of instrument for which the generator is to be returned.
             * @return generator Generator's definition.
             */
            std::shared_ptr<generator> get(const std::string& instrument_nm) const;
    };
}