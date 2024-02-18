#pragma once

#include <iostream>
#include "fin_curves.h"
#include "fin_generators.h"

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

    // generators
    {
        std::cout << "GENERATORS" << std::endl;
        std::vector<std::string> instrument_nms = {"BS_USDEUR_STFX", "EUR_3M_MANUAL", "EUR_ESTCRV_3M", "EUR ESTER CURVE",
                                                   "EUR ESTER FIXING", "EUR EURIBOR 3M RC", "EUR MM FIX", "STFX_FXD_USDEUR",
                                                   "USD_3M_MANUAL", "USD LIBOR 3M LCH", "USD LIBOR 3M RC", "USD MM FIX",
                                                   "USD SOFR CURVE", "USD SOFR FIXING"};
        fin_curves::myGenerators generators = fin_curves::myGenerators(instrument_nms);

        for (auto generator : generators.instruments)
            std::cout << generator.first << " - " << generator.second->instr_tp << std::endl;

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}