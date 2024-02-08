#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "lib_math.h"
#include "lib_str.h"

/*
 * ROUND
 */

double lib_math::round(const double& x, const unsigned short& decimals)
{
    unsigned int aux = pow(10, decimals);
    return ceil(x * aux) / aux;
}

/*
 * INTERPOLATION
 */

std::vector<double> lib_math::lin_interp1d(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& X, const bool extrapolate)
{
    // checks
    if (x.size() < 2)
        throw std::runtime_error((std::string)__func__ + ": Linear interpolation requires at least two points!");

    if (x.size() != y.size())
        throw std::runtime_error((std::string)__func__ + ": Length of x and y vectors must be the same!");

    if (X.size() == 0)
        throw std::runtime_error((std::string)__func__ + ": Vector X must contain at least one point!");

    // vector holding interpolated points Y
    std::vector<double> Y;

    // go through points X
    for (std::size_t X_idx = 0; X_idx < X.size(); X_idx++)
    {
        // lower values
        if (X[X_idx] < x[0])
        {
            // extrapolation
            if (extrapolate)
            {
                double x_low = x[0];
                double x_high = x[1];
                double y_low = y[0];
                double y_high = y[1];
                double Y_aux = y_low - (y_high - y_low) / (x_high - x_low) * (x_low - X[X_idx]);
                Y.push_back(Y_aux);
            }
            // no extrapolation
            else
                Y.push_back(y[0]);
        }
        // upper values
        else if (X[X_idx] > x[x.size() - 1])
        {
            // extrapolation
            if (extrapolate)
            {
                double x_low = x[x.size() - 2];
                double x_high = x[x.size() - 1];
                double y_low = y[x.size() - 2];
                double y_high = y[x.size() - 1];
                double Y_aux = y_high + (y_high - y_low) / (x_high - x_low) * (X[X_idx] - x_high);
                Y.push_back(Y_aux);
            }
            // no extrapolation
            else
                Y.push_back(y[y.size() - 2]);
        }
        // intermediate points
        else
        {
            for (std::size_t x_idx = 0; x_idx < x.size() - 1; x_idx++)
            {
                // exact match
                if (x[x_idx] == X[X_idx])
                    Y.push_back(y[x_idx]);
                // interpolate
                else
                {
                    if ((x[x_idx] < X[X_idx]) and (x[x_idx + 1] > X[X_idx]))
                    {
                        double x_low = x[x_idx];
                        double x_high = x[x_idx + 1];
                        double y_low = y[x_idx];
                        double y_high = y[x_idx + 1];
                        double Y_aux = y_low + (y_high - y_low) / (x_high - x_low) * (X[X_idx] - x_low);
                        Y.push_back(Y_aux);
                    }
                }

            }
        }
    }

    // return interpolated values
    return Y;
}

/*
 * NORMAL DISTRIBUTION
 */

// see: https://stackoverflow.com/questions/2328258/cumulative-normal-distribution-function-in-c-c
double lib_math::norm_cdf(const double& x)
{
    double M_SQRT_1_2 = std::sqrt(0.5);
    double cdf = 0.5 * std::erfc(-x * M_SQRT_1_2);
    return cdf;
}

double lib_math::norm_pdf(const double& x)
{
    double PI = 3.14159265358979;
    double pdf = 1 / std::sqrt(2 * PI) * std::exp(-0.5 * std::pow(x, 2));
    return pdf;
}

// see: https://link.springer.com/content/pdf/10.3758/BF03200956.pdf?pdf=button
double lib_math::norm_inv(const double& x)
{
    // define variables used in Odeh & Evans 1974 method
    double r;
    double z;
    double y;

    // take care of left / right tail
    if (x > 0.5)
        r = 1 - x;
    else
        r = x;

    // cut-off point
    if (r < 1e-20)
    {
        z = 10;
        if (x > 0.5)
            return z;
        else
            return -z;
    }

    // evaluate z
    y = std::sqrt(-2 * std::log(r));
    z = y - ((((4.53642210148E-5 * y + 0.0204231210245) * y + 0.342242088547) * y + 1) * y + 0.322232431088) /
            ((((0.0038560700634 * y + 0.10353775285) * y + 0.531103462366) * y + 0.588581570495) * y + 0.099348462606);


    // take care of left / right tail
    if (x > 0.5)
        return z;
    else
        return -z;
}

// see: https://stackoverflow.com/questions/2328258/cumulative-normal-distribution-function-in-c-c
std::vector<double> lib_math::norm_cdf(const std::vector<double>& x)
{
    double M_SQRT_1_2 = std::sqrt(0.5);
    std::vector<double> cdf;
    for (int idx = 0; idx < x.size(); idx++)
    {
        cdf.push_back(0.5 * std::erfc(-x[idx] * M_SQRT_1_2));
    }
    return cdf;
}

std::vector<double> lib_math::norm_pdf(const std::vector<double>& x)
{
    double PI = 3.14159265358979;
    std::vector<double> pdf;
    for (int idx = 0; idx < x.size(); idx++)
    {
        pdf.push_back(1 / std::sqrt(2 * PI) * std::exp(-0.5 * std::pow(x[idx], 2)));
    }
    return pdf;
}

// see: https://link.springer.com/content/pdf/10.3758/BF03200956.pdf?pdf=button
std::vector<double> lib_math::norm_inv(const std::vector<double>& x)
{
    std::vector<double> p;
    for (int idx = 0; idx < x.size(); idx++)
    {
        p.push_back(norm_inv(x[idx]));
    }
    return p;
}

/*
 * NEWTON-RAPHSON METHOD
 */

lib_math::newton_raphson_res lib_math::newton_raphson(double (*func)(double), double x, const double& step, const double& tolerance, const unsigned short& iter_max)
{
    // variables
    newton_raphson_res res;
    int iter_no = 0;
    double derivative;
    double x_down;
    double x_up;
    double func_down;
    double func_par;
    double func_up;

    // initiate Newton-Raphson structure
    res.x = x;
    res.step = step;
    res.tolerance = tolerance;
    res.iter_max = iter_max;

    do
    {
        // calculate derivation at initinal point x
        x_down = res.x - step;
        x_up = res.x + step;
        func_up = func(x_up);
        func_down = func(x_down);
        derivative = (func_up - func_down) / (2 * step);

        // apply Newton-Raphson method
        func_par = func(res.x);
        if (derivative == 0.0)
        {
            // return result after derivation being equal to zero
            res.success = false;
            res.iter_no = iter_no;
            res.msg = "zero derivation at point x = " + std::to_string(res.x);
            return res;
        }
        else if (std::abs(func_par) < tolerance)
        {
            // return result after reaching proximity of zero
            res.success = true;
            res.iter_no = iter_no;
            res.msg = "tolerance reached; f(x) = " + std::to_string(func_par);
            return res;
        }
        else
        {
            // iterate
            iter_no++;
            res.x += -func_par / derivative;
        }
    } while (iter_no < iter_max);

    // return result after exceeding maximum number of iterations
    res.success = false;
    res.iter_no = iter_no;
    res.msg = "maxium number of iterations reached";
    return res;

}

/*
 * DISCRETE PROBABILITY DISTRIBUTION OBJECT
 */

// constructor
lib_math::myDiscProbDist::myDiscProbDist(const std::vector<double>& x, const std::vector<double>& y)
{
    this->x = x;
    this->y = y;
}

// constructor
lib_math::myDiscProbDist::myDiscProbDist(const std::string& file_nm, const char& sep)
{
    // final vectors holding x and y values
    std::vector<double> x;
    std::vector<double> y;

    // row read from the .csv file
    std::string row;

    // vector of columns
    std::vector<std::string> splits;

    // open file
    std::ifstream f;
    f.open(file_nm, std::ios::out);

    // check that the file is indeed opened
    if (f.is_open())
    {
        // read the values row by row
        while (getline(f, row))
        {

            // retrieve values
            splits = lib_str::split_string(row, sep);
        
            x.push_back(std::stod(splits[0]));
            y.push_back(std::stod(splits[1]));
        }

        // close file
        f.close();

    }
    else
    {
        throw std::runtime_error((std::string)__func__ + ": Unable to open file " + file_nm + "!");
    }

    // initiate object variables
    this->x = x;
    this->y = y;
}

// return y-value based on x-value
double lib_math::myDiscProbDist::get(const double& x)
{
    // user specified x is lower or equal to the lowest x-value of the object
    if (x <= this->x[0])
        return this->y[0];

    // user specified x is higher or equal to the largest y-value of the object
    if (x >= this->x[this->x.size() - 1])
        return this->y[this->y.size() - 1];

    // find y-value corresponding to the user specified x
    for (unsigned short i = 0; i < this->x.size() - 2; i++)
    {
        if ((x >= this->x[i]) and (x < this->x[i + 1]))
            return this->y[i];
    }

    // no match found
    throw std::runtime_error((std::string)__func__ + ": No match found!");
}

std::vector<double> lib_math::myDiscProbDist::get(const std::vector<double>& x)
{
    // vector holding y-values
    std::vector<double> y;

    // go through x-values and assigned them corresponding y-values
    for (unsigned short i = 0; i < x.size(); i++)
        y.push_back(myDiscProbDist::get(x[i]));

    // return y-values
    return y;
}