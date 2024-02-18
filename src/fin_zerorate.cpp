#include "lib_math.h"
#include "lib_tbl.h"
#include "fin_curves.h"
#include "fin_zerorate.h"

fin_curves::myZeroRate::myZeroRate(const fin_curves::interp_def& interp_def)
{
    // copy definition of curve interpolation
    this->interp_def = interp_def;
}

fin_curves::myZeroRate::myZeroRate(const std::string& path, const std::string& crv_nm, const std::string& sep, const bool& quotes)
{
    // store variables into the object
    this->path = path;
    this->crv_nm = crv_nm;
    this->sep = sep;
    this->quotes = quotes;

    // read file with curve interpolation definition
    lib_tbl::myTable crv_interp_def;
    crv_interp_def.read(this->path + "inputs/curves/" + this->crv_nm + ".csv", this->sep, this->quotes);

    // extract interpolation definition
    fin_curves::interp_def interp_def;
    interp_def.ccy = *crv_interp_def.get_str_column("ccy")[0];
    interp_def.zr_dcm = *crv_interp_def.get_str_column("zr_dcm")[0];
    interp_def.zr_interp = *crv_interp_def.get_str_column("zr_interp")[0];
    interp_def.zr_type = *crv_interp_def.get_int_column("zr_type")[0];
    this->interp_def = interp_def;
}

void fin_curves::myZeroRate::load(const std::vector<double>& yr_fracs, const std::vector<double>& zrs)
{
    // check that vector of year fractions has the same lenght as vector of zero rates
    if (yr_fracs.size() != zrs.size())
        throw std::runtime_error((std::string)__func__ + ": Vector of year fractions and vector of zero rates are of different length!");

    // copy year fractions and corresponding zero rates
    this->yr_fracs_aux.clear();
    this->yr_fracs.clear();
    this->yr_fracs = yr_fracs;
    this->zrs_aux.clear();
    this->zrs.clear();
    this->zrs = zrs;
}

void fin_curves::myZeroRate::load(const std::string& date, const std::string& date_format)
{
    // convert date to yyyymmdd if necessary
    std::string date_str = date;
    if (date_format.compare("yyyymmdd") == 1)
    {
        lib_date::myDate date_aux(date, date_format);
        date_str = date_aux.get_date_str();
    }

    // prepare myTable object
    lib_tbl::myTable crv_interp_def;
    crv_interp_def.read(this->path + "outputs/curves/" + this->crv_nm + "_" + date_str + ".csv", this->sep, this->quotes);

    // copy year fractions and corresponding zero rates
    this->yr_fracs_aux.clear();
    this->yr_fracs_aux = crv_interp_def.get_double_column("yr_fracs");
    this->zrs_aux.clear();
    this->zrs_aux = crv_interp_def.get_double_column("zrs");

    // check that vector of year fractions has the same lenght as vector of zero rates
    if (this->yr_fracs.size() != this->zrs.size())
        throw std::runtime_error((std::string)__func__ + ": Vector of year fractions and vector of zero rates are of different length!");

    // interpolate potentially missing rates
    std::tuple<std::vector<double>, std::vector<double>> aux = fin_curves::interp_rate(this->yr_fracs_aux, this->zrs_aux);
    this->yr_fracs = std::get<0>(aux);
    this->zrs = std::get<1>(aux);
}

double fin_curves::myZeroRate::get_zr(const double& yr_frac) const
{
    // define interpolated variables
    std::vector<double> yr_frac_aux = std::vector<double>({yr_frac});
    std::vector<double> zr_aux;

    // interpolate
    if (this->interp_def.zr_interp.compare("linear") == 0)
        zr_aux = lib_math::lin_interp1d(this->yr_fracs, this->zrs, yr_frac_aux, false); // false => we do not extrapolate
    else
        throw std::runtime_error((std::string)__func__ + ": '" + this->interp_def.zr_interp + "' is not a supported interpolation!");

    // return interpolated zero rate
    return zr_aux[0];
}

double fin_curves::myZeroRate::get_df(const double& yr_frac) const
{
    double zr = this->get_zr(yr_frac);
    return fin_curves::zr_to_df(zr, yr_frac, this->interp_def.zr_type);
}

void fin_curves::myZeroRate::reset()
{
    this->yr_fracs.clear();
    this->yr_fracs_aux.clear();
    this->zrs.clear();
    this->zrs_aux.clear();
}