#include "lib_str.h"
#include "fin_generators.h"
#include "fin_depoinstr.h"

int main()
{
    // create generator
    std::string instrument_nm = "EUR_3M_MANUAL";
    std::vector<std::string> instrument_nms = {instrument_nm};
    fin_curves::myGenerators generators = fin_curves::myGenerators(instrument_nms);
    std::shared_ptr<fin_curves::generator> generator = generators.get(instrument_nm);

    // underlying zero rate curve
    std::string path = "/home/macky/Documents/Programming/C++/FinCurves/data/";
    std::string crv_nm = "eur_ri_3m_fo";
    std::string sep = ",";
    fin_curves::myZeroRate crv = fin_curves::myZeroRate(path, crv_nm, sep);

    // calculate discount factors deposit instrument
    std::string date_exec = "20220531";
    std::string date_format = "yyyymmdd";
    std::vector<std::string> tenors = {"ON", "TN", "1M", "2M"};
    std::vector<double> par_rates = {-0.008452, -0.008452, -0.006815, -0.005172};

    for (std::size_t idx = 0; idx < tenors.size(); idx++)
        fin_curves::myDepoInstr depo = fin_curves::myDepoInstr(date_exec, tenors[idx], par_rates[idx], generator, crv, date_format);

    // retrieve calculated figures
    std::vector<double> yr_fracs = crv.get_yr_fracs();
    std::vector<double> dfs;
    for (std::size_t idx = 0; idx < yr_fracs.size(); idx++)
    {
        dfs.push_back(crv.get_df(yr_fracs[idx]));
    }

    // print calculate discount factors
    std::cout << instrument_nm << std::endl;
    std::cout << lib_str::repeate_char("=", 45) << std::endl;
    std::cout << "|tenor  " << "|year fraction" << "|par rate  " << "|df        " << "|" << std::endl;
    std::cout << lib_str::repeate_char("=", 45) << std::endl;
    for (std::size_t idx = 0; idx < yr_fracs.size(); idx++)
    {
        std::cout << "|" << lib_str::add_trailing_char(tenors[idx], 7) << "|" << lib_str::add_trailing_char(std::to_string(yr_fracs[idx]), 13);
        std::cout << "|" << lib_str::add_trailing_char(std::to_string(par_rates[idx]), 10);
        std::cout << "|" << lib_str::add_trailing_char(std::to_string(dfs[idx]), 10);
        std::cout << "|" << std::endl;
    }
    std::cout << lib_str::repeate_char("=", 45) << std::endl;

    // everything OK
    return 0;
}