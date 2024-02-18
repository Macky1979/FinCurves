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

    // depo
    {
        std::cout << "DEPO" << std::endl;

        std::string date_exec = "20240109";
        std::string tenor = "1Y";
        double quote = 0.01;
        std::string crv_nm = "--na--";
        std::string date_format = "yyyymmdd";

        std::vector<std::string> instrument_nms = {"USD_3M_MANUAL"};
        fin_curves::myGenerators generators = fin_curves::myGenerators(instrument_nms);
        std::shared_ptr<fin_curves::generator> generator = generators.get("USD_3M_MANUAL");

        fin_curves::myDepoInstr depo = fin_curves::myDepoInstr(date_exec, tenor, quote, crv_nm, generator, date_format);

        std::vector<std::tuple<lib_date::myDate, lib_date::myDate>> dates = depo.get_date_series();
        for (std::size_t idx = 0; idx < dates.size(); idx++)
        {
            std::cout << std::get<0>(dates[idx]).get_date_str() << " - " << std::get<1>(dates[idx]).get_date_str() << std::endl;
        }

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}