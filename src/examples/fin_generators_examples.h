#include <iostream>
#include "lib_str.h"
#include "lib_date.h"
#include "fin_generators.h"

int main()
{
    std::cout << "GENERATORS" << std::endl;

    // list of generators
    std::vector<std::string> instrument_nms = {"BS_USDEUR_STFX", "EUR_3M_MANUAL", "EUR_ESTCRV_3M", "EUR ESTER CURVE",
                                                "EUR ESTER FIXING", "EUR EURIBOR 3M RC", "EUR MM FIX", "STFX_FXD_USDEUR",
                                                "USD_3M_MANUAL", "USD LIBOR 3M LCH", "USD LIBOR 3M RC", "USD MM FIX",
                                                "USD SOFR CURVE", "USD SOFR FIXING"};

    // load generators into a object
    fin_curves::myGenerators generators = fin_curves::myGenerators(instrument_nms);

    // table header
    std::cout << std::string(50, '=') << std::endl;
    std::cout << lib_str::add_trailing_char("name", 20) << lib_str::add_trailing_char("type", 10) << lib_str::add_trailing_char("day roll method", 20) << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    // go generator by generator and print basic information
    for (auto generator : generators.instruments)
    {
        std::cout << lib_str::add_trailing_char(generator.first, 20) << lib_str::add_trailing_char(generator.second->instr_tp, 10);
        if (generator.second->instr_tp.compare("depo") == 0)
            std::cout << lib_str::add_trailing_char(generator.second->instr_def.depo.drm, 20) << std::endl;
        else if (generator.second->instr_tp.compare("fra") == 0)
            std::cout << lib_str::add_trailing_char(generator.second->instr_def.fra.drm, 20) << std::endl;
        else if (generator.second->instr_tp.compare("ois") == 0)
            std::cout << lib_str::add_trailing_char(generator.second->instr_def.ois.drm, 20) << std::endl;
        else if (generator.second->instr_tp.compare("irswp") == 0)
            std::cout << lib_str::add_trailing_char(generator.second->instr_def.irswp.drm, 20) << std::endl;
        else if (generator.second->instr_tp.compare("bsswp") == 0)
            std::cout << lib_str::add_trailing_char(generator.second->instr_def.bsswp.drm, 20) << std::endl;
        else
            std::cout << "--na--" << std::endl;
    }

    std::cout << '\n' << std::endl;

    // everything OK
    return 0;
}