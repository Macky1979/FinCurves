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
#include "fin_zerorate.h"
#include "fin_generators.h"

namespace fin_curves
{
    /**
     * @brief Object evaluating depo instrument upon its creation.
     * 
     */
    class myDepoInstr
    {
        private:
            double yr_frac_int;
            double yr_frac_pmt;
            double par_rate;
            double df;

        public:
            /**
             * @brief Construct a new myDepoInstr object and evaluate depo instrument, i.e. calculate its maturity year fraction and discount factor.
             * 
             * @param date Date from which depo maturity is derived using tenor information.
             * @param tenor Frequency string defining depo maturity, e.g. "ON", "TN", "1W", "1M", "1Y", etc.
             * @param par_rate Depo rate quoted on market for the respective maturity tenor as of execution date.
             * @param generator Generator with definitions needed to evaluate depo instrument.
             * @param crv Zero rate curve which is used to store depo maturity year fraction and related zero rate derived from discount factor.
             * @param date_format Date format.
             */
            myDepoInstr(const std::string& date_exec, const std::string& tenor, const double& par_rate, const std::shared_ptr<fin_curves::generator>& generator, fin_curves::myZeroRate& crv, std::string date_format = "yyyymmdd");
    };
}