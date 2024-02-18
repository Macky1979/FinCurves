#include <iostream>
#include <vector>
#include <memory>
#include "lib_tbl.h"
#include "fin_generators.h"

fin_curves::myGenerators::myGenerators(const std::vector<std::string>& instrument_nms, const std::string& sep, const bool& quotes)
{
    // clear old information
    this->instruments.clear();

    // prepare myTable object
    lib_tbl::myTable instrument_def;

    // load generator by generator
    for (std::string instrument_nm : instrument_nms)
    {
        // load file with instrument definition
        std::string file_nm = "/home/macky/Documents/Programming/C++/FinCurves/data/inputs/generators/" + instrument_nm + ".csv";
        instrument_def.read(file_nm, sep, quotes);

        // select specific generator
        std::string instrument_tp = instrument_def.tbl.values[0][0];
        if (instrument_tp.compare("depo") == 0)
        {
            fin_curves::depo depo;
            depo.cal_pmt = *instrument_def.get_str_column("cal_pmt")[0];
            depo.drm = *instrument_def.get_str_column("drm")[0];
            depo.dcm = *instrument_def.get_str_column("dcm")[0];
            depo.days_shift = *instrument_def.get_int_column("days_shift")[0];

            std::shared_ptr<fin_curves::generator> generator = std::make_shared<fin_curves::generator>();
            generator->instr_nm = instrument_nm;
            generator->instr_tp = instrument_tp;
            generator->instr_def.depo = depo;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_nm, generator));
        }
        else if (instrument_tp.compare("fra") == 0)
        {
            fin_curves::fra fra;
            fra.cal_fix = *instrument_def.get_str_column("cal_fix")[0];
            fra.cal_pmt = *instrument_def.get_str_column("cal_pmt")[0];
            fra.drm = *instrument_def.get_str_column("drm")[0];
            fra.dcm = *instrument_def.get_str_column("dcm")[0];
            fra.days_shift = *instrument_def.get_int_column("days_shift")[0];
            fra.period = *instrument_def.get_str_column("period")[0];

            std::shared_ptr<fin_curves::generator> generator = std::make_shared<fin_curves::generator>();
            generator->instr_nm = instrument_nm;
            generator->instr_tp = instrument_tp;
            generator->instr_def.fra = fra;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_nm, generator));
        }
        else if (instrument_tp.compare("ois") == 0)
        {
            fin_curves::ois ois;
            ois.cal_fix = *instrument_def.get_str_column("cal_fix")[0];
            ois.cal_pmt = *instrument_def.get_str_column("cal_pmt")[0];
            ois.drm = *instrument_def.get_str_column("drm")[0];
            ois.days_shift = *instrument_def.get_int_column("days_shift")[0];
            ois.simple_vs_compounding = *instrument_def.get_str_column("simple_vs_compounding")[0];
            ois.fix_leg_dcm = *instrument_def.get_str_column("fix_leg_dcm")[0];
            ois.flt_leg_dcm = *instrument_def.get_str_column("flt_leg_dcm")[0];

            std::shared_ptr<fin_curves::generator> generator = std::make_shared<fin_curves::generator>();
            generator->instr_nm = instrument_nm;
            generator->instr_tp = instrument_tp;
            generator->instr_def.ois = ois;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_nm, generator));
        }
        else if (instrument_tp.compare("irswp") == 0)
        {
            fin_curves::irswp irswp;
            irswp.drm = *instrument_def.get_str_column("drm")[0];
            irswp.fix_leg_cal_pmt = *instrument_def.get_str_column("fix_leg_cal_pmt")[0];
            irswp.fix_leg_dcm = *instrument_def.get_str_column("fix_leg_dcm")[0];
            irswp.fix_leg_freq = *instrument_def.get_str_column("fix_leg_freq")[0];
            irswp.fix_leg_days_shift = *instrument_def.get_int_column("fix_leg_days_shift")[0];
            irswp.flt_leg_cal_fix = *instrument_def.get_str_column("flt_leg_cal_fix")[0];
            irswp.flt_leg_cal_pmt = *instrument_def.get_str_column("flt_leg_cal_pmt")[0];
            irswp.flt_leg_dcm = *instrument_def.get_str_column("flt_leg_dcm")[0];
            irswp.flt_leg_freq = *instrument_def.get_str_column("flt_leg_freq")[0];
            irswp.flt_leg_days_shift = *instrument_def.get_int_column("flt_leg_days_shift")[0];

            std::shared_ptr<fin_curves::generator> generator = std::make_shared<fin_curves::generator>();
            generator->instr_nm = instrument_nm;
            generator->instr_tp = instrument_tp;
            generator->instr_def.irswp = irswp;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_nm, generator));
        }
        else if (instrument_tp.compare("bsswp") == 0)
        {
            fin_curves::bsswp bsswp;
            bsswp.drm = *instrument_def.get_str_column("drm")[0];
            bsswp.dom_leg_cal_fix = *instrument_def.get_str_column("dom_leg_cal_fix")[0];
            bsswp.dom_leg_cal_pmt = *instrument_def.get_str_column("dom_leg_cal_pmt")[0];
            bsswp.dom_leg_dcm = *instrument_def.get_str_column("dom_leg_dcm")[0];
            bsswp.dom_leg_freq = *instrument_def.get_str_column("dom_leg_freq")[0];
            bsswp.dom_leg_days_shift = *instrument_def.get_int_column("dom_leg_days_shift")[0];
            bsswp.for_leg_cal_fix = *instrument_def.get_str_column("for_leg_cal_fix")[0];
            bsswp.for_leg_cal_pmt = *instrument_def.get_str_column("for_leg_cal_pmt")[0];
            bsswp.for_leg_dcm = *instrument_def.get_str_column("for_leg_dcm")[0];
            bsswp.for_leg_freq = *instrument_def.get_str_column("for_leg_freq")[0];
            bsswp.for_leg_days_shift = *instrument_def.get_int_column("for_leg_days_shift")[0];

            std::shared_ptr<fin_curves::generator> generator = std::make_shared<fin_curves::generator>();
            generator->instr_nm = instrument_nm;
            generator->instr_tp = instrument_tp;
            generator->instr_def.bsswp = bsswp;
            this->instruments.insert(std::pair<std::string, std::shared_ptr<fin_curves::generator>>(instrument_nm, generator));
        }
    }
}

std::shared_ptr<fin_curves::generator> fin_curves::myGenerators::get(const std::string& instrument_nm) const
{
    return this->instruments.find(instrument_nm)->second;
}