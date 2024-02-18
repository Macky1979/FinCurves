#include <iostream>
#include <vector>
#include "fin_date.h"
#include "fin_depoinstr.h"


fin_curves::myDepoInstr::myDepoInstr(const std::string& crv_nm, const std::shared_ptr<fin_curves::generator>& generator, const std::string& date_exec, const std::vector<std::string>& tenor, const std::vector<double>& quote, const std::string& date_format)
{
    // check that the generator is depo
    if (generator->instr_tp.compare("depo") == 1)
        throw std::runtime_error((std::string)__func__ + ": Instrument '" + generator->instr_nm + "' is not a deposit!");

    // store selected information into the object
    this->date_exec = date_exec;
    this->tenors = tenors;
    this->quotes= quotes;
    this->generator = generator;
    this->crv_nm = crv_nm;

    // generate date series
    this->date_serie = fin_date::create_date_serie(this->date_exec, this->tenors, this->generator->instr_def.depo.cal_pmt, this->generator->instr_def.depo.drm, date_format);

    // go tenor by tenor
    double yr_frac;
    double zr;
    for (std::size_t idx; idx < this->tenors.size(); idx++)
    {
        // determine auxiliary discount factors

        if (this->tenors[idx].compare("ON") == 0)
        {
            yr_frac = 1.0;
            zr = 0.0;
        }
        else if (this->tenors[idx].compare("TN") == 0)
        {
            yr_frac = 0.0; // this->std::get<0>date_serie[idx - 1]; 
        }
    }


}