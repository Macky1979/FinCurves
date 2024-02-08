#include <cmath>
#include <vector>
#include <stdexcept>
#include <lib_tbl.h>
#include "fin_curves.h"


double fin_curves::df_to_zr(const double& df, const double& yr_frac, const unsigned short type)
{
    // df = 1 / (1 + zr) ^ yr_frac
    if (type == 0)
    {
        return std::pow(df, -1.0 / yr_frac) - 1.0;
    }
    // df = exp(-zr * yr_frac)
    else if (type == 1)
    {
        return -1.0 * std::log(df) / yr_frac;
    }
    // unsupported conversion type
    else
        throw std::runtime_error((std::string)__func__ + ": '" + std::to_string(type) + "' is not a supported conversion type!");
}

std::vector<double> fin_curves::df_to_zr(const std::vector<double>& dfs, const std::vector<double>& yr_fracs, const unsigned short type)
{
    // vector of zero rates
    std::vector<double> zrs;

    // go discount factor by discount factor
    for (std::size_t idx = 0; idx < dfs.size(); idx++)
    {
        zrs.push_back(df_to_zr(dfs[idx], yr_fracs[idx], type));
    }

    // return vector of zero rates
    return zrs;
}

fin_curves::myGenerators::myGenerators(const std::vector<std::string>& instrument_nms)
{
    // clear old information
    this->instruments.clear();

    // prepare myTable object
    std::string sep = ",";
    bool quotes = false;
    lib_tbl::myTable instrument_def;

    // load generator by generator
    for (std::string instrument_nm : instrument_nms)
    {
        // load file with instrument definition
        std::string file_nm = "../data/inputs/generators/" + instrument_nm + ".csv";
        instrument_def.read(file_nm, sep, quotes);

        // select specific generator
        std::string instrument_tp = instrument_def.tbl.values[0][0];
        if (instrument_tp.compare("depo") == 0)
        {
            fin_curves::depo_generator depo;
            depo.cal_pmt = *instrument_def.get_str_column("cal_pmt")[0];
            depo.drm = *instrument_def.get_str_column("drm")[0];
            depo.dcm = *instrument_def.get_str_column("dcm")[0];
            depo.days_shift = *instrument_def.get_int_column("days_shift")[0];

            std::shared_ptr<fin_curves::generator> generator;
            generator->depo = depo;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_tp, generator));
        }
        else if (instrument_tp.compare("fra") == 0)
        {
            fin_curves::fra_generator fra;
            fra.cal_fix = *instrument_def.get_str_column("cal_fix")[0];
            fra.cal_pmt = *instrument_def.get_str_column("cal_pmt")[0];
            fra.drm = *instrument_def.get_str_column("drm")[0];
            fra.dcm = *instrument_def.get_str_column("dcm")[0];
            fra.days_shift = *instrument_def.get_int_column("days_shift")[0];
            fra.period = *instrument_def.get_str_column("period")[0];

            std::shared_ptr<fin_curves::generator> generator;
            generator->fra = fra;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_tp, generator));
        }
        else if (instrument_tp.compare("ois") == 0)
        {
            fin_curves::ois_generator ois;
            ois.cal_fix = *instrument_def.get_str_column("cal_fix")[0];
            ois.cal_pmt = *instrument_def.get_str_column("cal_pmt")[0];
            ois.drm = *instrument_def.get_str_column("drm")[0];
            ois.days_shift = *instrument_def.get_int_column("days_shift")[0];
            ois.simple_vs_compounding = *instrument_def.get_str_column("simple_vs_compounding")[0];
            ois.fix_leg_dcm = *instrument_def.get_str_column("fix_leg_dcm")[0];
            ois.flt_leg_dcm = *instrument_def.get_str_column("flt_leg_dcm")[0];

            std::shared_ptr<fin_curves::generator> generator;
            generator->ois = ois;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_tp, generator));
        }
        else if (instrument_tp.compare("irswp") == 0)
        {
            fin_curves::irswp_generator irswp;
            irswp.drm = *instrument_def.get_str_column("drm")[0];
            irswp.fix_leg_cal_pmt = *instrument_def.get_str_column("fix_leg_cal_pmt")[0];
            irswp.fix_leg_dcm = *instrument_def.get_str_column("fix_leg_dcm")[0];
            irswp.fix_leg_freq = *instrument_def.get_str_column("fix_leg_freq")[0];
            irswp.fix_leg_days_shift = *instrument_def.get_int_column("fix_leg_days_shift")[0];
            irswp.flt_leg_cal_pmt = *instrument_def.get_str_column("flt_leg_cal_pmt")[0];
            irswp.flt_leg_dcm = *instrument_def.get_str_column("flt_leg_dcm")[0];
            irswp.flt_leg_freq = *instrument_def.get_str_column("flt_leg_freq")[0];
            irswp.flt_leg_days_shift = *instrument_def.get_int_column("flt_leg_days_shift")[0];

            std::shared_ptr<fin_curves::generator> generator;
            generator->irswp = irswp;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_tp, generator));
        }
        else if (instrument_tp.compare("bsswp") == 0)
        {
            fin_curves::bsswp_generator bsswp;
            bsswp.drm = *instrument_def.get_str_column("drm")[0];
            bsswp.dom_leg_cal_fix = *instrument_def.get_str_column("dom_leg_cal_fix")[0];
            bsswp.dom_leg_cal_pmt = *instrument_def.get_str_column("dom_leg_cal_pmt")[0];
            bsswp.dom_leg_dcm = *instrument_def.get_str_column("dom_leg_dcm")[0];
            bsswp.dom_leg_days_shift = *instrument_def.get_int_column("dom_leg_days_shift")[0];
            bsswp.dom_leg_freq = *instrument_def.get_str_column("dom_leg_freq")[0];
            bsswp.for_leg_cal_fix = *instrument_def.get_str_column("for_leg_cal_fix")[0];
            bsswp.for_leg_cal_pmt = *instrument_def.get_str_column("for_leg_cal_pmt")[0];
            bsswp.for_leg_dcm = *instrument_def.get_str_column("for_leg_dcm")[0];
            bsswp.for_leg_days_shift = *instrument_def.get_int_column("for_leg_days_shift")[0];
            bsswp.for_leg_freq = *instrument_def.get_str_column("for_leg_freq")[0];

            std::shared_ptr<fin_curves::generator> generator;
            generator->bsswp = bsswp;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_tp, generator));
        }
    }
}