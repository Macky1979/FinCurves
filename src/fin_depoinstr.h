/** \example fin_depoinstr_examples.h
 * @file fin_depoinstr.h
 * @author Michal Mackanic
 * @brief Implement depo instrument.
 * @version 1.0
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
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
     * @brief Object holding depo instrument.
     * 
     */
    class myDepoInstr
    {
        private:
            std::string crv_nm;
            std::shared_ptr<fin_curves::generator> generator;
            std::string date_exec;
            std::vector<std::string> tenors;
            std::vector<double> quotes;
            std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>> date_serie;
            std::tuple<lib_date::myDate, lib_date::myDate> dates_int;

        public:
            myDepoInstr(const std::string& crv_nm, const std::shared_ptr<fin_curves::generator>& generator, const std::string& date_exec, const std::vector<std::string>& tenor, const std::vector<double>& quote, const std::string& date_format = "yyyymmdd");
            std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>> get_date_series(){return this->date_serie;};
    };
}