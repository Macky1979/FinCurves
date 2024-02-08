/** \example lib_math_examples.h
 * @file lib_math.h
 * @author Michal Mackanič
 * @brief 
 * @version 1.0
 * @date 2024-01-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <vector>

namespace lib_math
{
    /**
     * @brief Round double to user-specified number of digits.
     * 
     * @param x Number to be rounded.
     * @param decimals Number of decimals to which the variable x is supposed to be rounded.
     * @return double Rounded number.
     */
    double round(const double& x, const unsigned short &decimals = 2);

    /**
     * @brief Linear 1D interpolation.
     * 
     * @param x Vector of x points ordered from lower to higher.
     * @param y Vector of y points corresponding to individual x points.
     * @param X Vector of X points for which Y points should be interpolated based on x and y points.
     * @param extrapolate true => Extrapolate end points. \n false => Do not extrapolate end points.
     * @return std::vector<double> Vector of interpolated points Y.
     */
    std::vector<double> lin_interp1d(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& X, const bool extrapolate = false);

    /** 
     * @brief Standardized normal CDF evaluated at a single point.
     * 
     * @param x Point at which standardized normal CDF is to be evaluated.
     * @return double Value of standardized normal CDF at the point.
     */
    double norm_cdf(const double& x);

    /** 
     * @brief Standaridized normal PDF evaluated at a single point.
     * 
     * @param x Point at which standardized normal PDF is to be evaluated.
     * @return double Value of standardized normal PDF at the point.
     */
    double norm_pdf(const double& x);

    /** 
     * @brief Inverse standardized normal distribution evaluated at a single quantile.
     * 
     * @param x Quantile at which inverse standardized normal distribution is to be evaluated.
     * @return double Value of inverse standardized normal distribution at the quantile.
     */
    double norm_inv(const double& x);

    /** 
     * @brief Standardized normal CDF evaluated for a vector of points.
     * 
     * @param x Vector of points at which standardized normal CDF is to be evaluated. 
     * @return std::vector<double> Values of standardized normal CDF at the individual points.
     */
    std::vector<double> norm_cdf(const std::vector<double>& x);

    /** 
     * @brief Standardized normal PDF evaluated for a vector of points.
     * 
     * @param x Vector of points at which standardized normal PDF is to be evaluated.
     * @return std::vector<double> Values of standardized normal PDF at the individual points.
     */
    std::vector<double> norm_pdf(const std::vector<double>& x);

    /** 
     * @brief Inverse standardized normal distribution evaluated for a vector of quantiles.
     * 
     * @param x Vector of quantiles at which inverse standardized normal distribuiton is to be evaluated.
     * @return std::vector<double> Values of inverse standardized normal distribution at the individual quantiles.
     */
    std::vector<double> norm_inv(const std::vector<double>& x);

    /** 
     * @brief Data structure holding results of Newton-Raphson method.
     * 
     */
    struct newton_raphson_res
    {
        double x;
        bool success;
        double step;
        double tolerance;
        int iter_no;
        int iter_max;
        std::string msg;
    };

    /** 
     * @brief Implementation of Newton-Raphson method.
     * 
     * @param func Single variable function for which we search x such that f(x) = 0 using Newton-Raphson method.
     * @param x Initial estimate of optimal value of variable x.
     * @param step Lenght of interation step to applied within Newton-Raphson method.
     * @param tolerance Interation process is stopped if value of the function is in range of +/- tolerance.
     * @param iter_max Maximum number of iterations.
     * @return newton_raphson_res Data structure holding results of Newton-Raphosn method.
     */
    newton_raphson_res newton_raphson(double (*func)(double), double x, const double& step, const double& tolerance, const unsigned short& iter_max);

    /** 
     * @brief Discrete probability distribution.
     * 
     * The object maps x to y = f(x). In this way, CDF, PDF and inverse probability functions could be implemented.
     */
    class myDiscProbDist
    {
        private:
            std::vector<double> x;
            std::vector<double> y;
        
        public:

            /** 
             * @brief Construct a new myDiscProbDist object using vectors of x and y.
             * 
             * @param x Vector of x points.
             * @param y Vector of y points.
             */
            myDiscProbDist(const std::vector<double>& x, const std::vector<double>& y);

            /** 
             * @brief Construct a new myDiscProbDist object using information on x and y stored in a .csv file.
             * 
             * @param file_nm Name of a .csv file holding information on x points (the first column) and their corresponding y points (the second column).
             * @param sep Column separator used in the .csv file.
             */
            myDiscProbDist(const std::string& file_nm, const char& sep = ',');

            /** 
             * @brief Destroy the myDiscProbDist object.
             * 
             */
            ~myDiscProbDist(){};

            /** 
             * @brief Get f(x) for a single point x.
             * 
             * @param x Single point x.
             * @return double Single point y = f(x).
             */
            double get(const double& x);

            /** 
             * @brief Get vector of f(x) values corresponding to vector of x values.
             * 
             * @param x Vector of x points.
             * @return std::vector<double> Vector of y = f(x) points.
             */
            std::vector<double> get(const std::vector<double>& x);
    };
}