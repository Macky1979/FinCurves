#include <iostream>
#include "fin_zerorate.h"

int main()
{
    std::cout << "ZERO RATES" << std::endl;

    // load zero curve interpolation definition
    std::cout << "Loading curve interpolation definition..." << std::endl;
    std::string path = "/home/macky/Documents/Programming/C++/FinCurves/data/";
    std::string crv_nm = "eur_ri_3m_fo";
    std::string sep = ",";
    bool quotes = false;  // no quotes used in .csv file to "enclose" text
    fin_curves::myZeroRate eur_ri_3m_fo = fin_curves::myZeroRate(path, crv_nm, sep, quotes);

    fin_curves::interp_def interp_def = eur_ri_3m_fo.get_def();
    std::cout << "   " << "curve name: " << eur_ri_3m_fo.get_crv_nm() << std::endl;
    std::cout << "   " << "currency: " << interp_def.ccy << std::endl;
    std::cout << "   " << "day-count method: " << interp_def.zr_dcm << std::endl;
    std::cout << "   " << "zero rate type: " << std::to_string(interp_def.zr_type) << std::endl;
    std::cout << "   " << "interpolation definition: " << interp_def.zr_interp << std::endl;

    // load zero rates
    std::cout << "Loading year fractions and corresponding zero rates..." << std::endl;
    std::string date = "20220531";
    std::string date_format = "yyyymmdd";
    eur_ri_3m_fo.load(date, date_format);

    // define variable necessary to calculate zero rate and discount factor
    double yr_frac;
    double zr;
    double df;

    // get zero rate and discount factor as of 2 years from 31/05/2022
    yr_frac = 2.0;
    zr = eur_ri_3m_fo.get_zr(yr_frac);
    df = eur_ri_3m_fo.get_df(yr_frac);
    std::cout << "   year fraction: " << std::to_string(yr_frac) << " - zero rate: " << std::to_string(zr) << ", discount factor: " << std::to_string(df) << std::endl;

    // clear object
    std::cout << "Clearing object..." << std::endl;
    eur_ri_3m_fo.reset();

    // re-loading object
    std::cout << "Reloading object..." << std::endl;

    std::vector<double> yr_fracs = {0.0931507, 0.2575342, 0.5068493, 0.7534247, 1.0054795, 2.0109589, 3.0082192, 4.0082192, 5.0082192};
    std::vector<double> zrs = {-0.0069821, -0.0034579, -0.0003441, 0.0022772, 0.0047364, 0.0106236, 0.0127052, 0.0139706, 0.0149029};
    eur_ri_3m_fo.load(yr_fracs, zrs);

    yr_frac = 2.0;
    zr = eur_ri_3m_fo.get_zr(yr_frac);
    df = eur_ri_3m_fo.get_df(yr_frac);
    std::cout << "   year fraction: " << std::to_string(yr_frac) << " - zero rate: " << std::to_string(zr) << ", discount factor: " << std::to_string(df) << std::endl;

    std::cout << '\n' << std::endl;

    // everything OK
    return 0;
}