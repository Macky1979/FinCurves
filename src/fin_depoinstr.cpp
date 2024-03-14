#include <iostream>
#include <tuple>
#include <vector>
#include "fin_date.h"
#include "fin_zerorate.h"
#include "fin_depoinstr.h"

fin_curves::myDepoInstr::myDepoInstr(const std::string& date, const std::string& tenor, const double& par_rate, const std::shared_ptr<fin_curves::generator>& generator, fin_curves::myZeroRate& crv, std::string date_format)
{

    // check that the generator is depo
    if (generator->instr_tp.compare("depo") == 1)
        throw std::runtime_error((std::string)__func__ + ": Instrument '" + generator->instr_nm + "' is not a deposit!");

    // store par rate
    this->par_rate = par_rate;

    // generate list of tenors
    std::vector<std::string> tenors;
    if (tenor.compare("ON") == 0)
        tenors.push_back("ON");
    else if (tenor.compare("TN") == 0)
    {
        tenors.push_back("ON");
        tenors.push_back("TN");
    }
    else
    {
        tenors.push_back("ON");
        tenors.push_back("TN");
        tenors.push_back(tenor);
    }

    // generate date series
    std::tuple<std::vector<lib_date::myDate>, std::vector<lib_date::myDate>> dates = fin_date::create_date_serie(date, tenors, generator->instr_def.depo.cal_pmt, generator->instr_def.depo.drm, date_format);

    // determine auxiliary discount factor
    std::size_t idx;
    double df_aux;
    if (tenor.compare("ON") == 0)
        df_aux = 1.0;
    else if (tenor.compare("TN") == 0)
    {
        idx = 0;
        df_aux = crv.get_df(idx);
    }
    else
    {
        idx = 1;
        df_aux = crv.get_df(idx);
    }

    // calculate year fraction for interest period
    std::size_t tenors_no = std::get<0>(dates).size();
    lib_date::myDate date_begin = std::get<0>(dates)[tenors_no - 1];
    lib_date::myDate date_end = std::get<1>(dates)[tenors_no - 1];
    this->yr_frac_int = fin_date::day_count_method(date_begin, date_end, generator->instr_def.depo.dcm);

    // calculate discount factor
    this->df = 1.0 / (1.0 + this->par_rate * this->yr_frac_int) * df_aux;

    // calculate year fraction for payment period
    this->yr_frac_pmt = fin_date::day_count_method(lib_date::myDate(date, date_format), date_end, generator->instr_def.depo.dcm);

    // convert discount factor into zero rate following convention of the underlying zero rate curve
    double zr = fin_curves::df_to_zr(this->df, this->yr_frac_pmt, crv.get_def().zr_type);

    // add year fraction and zero rate to the underlying zero rate curve
    std::tuple<double, double> yr_frac_zr = {this->yr_frac_pmt, zr};
    crv.add(yr_frac_zr);
}