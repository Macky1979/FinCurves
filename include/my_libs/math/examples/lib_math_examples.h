#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "lib_math.h"

double sqrt_of_2(double x)
{
    return (2 - std::pow(x, 2));
}

int main()
{

    // ROUND
    {
        std::cout << "ROUND" << std::endl;
        double pi = 3.141592653589793238462643383279502884197;
        std::cout << std::to_string(pi) << " -> " << std::to_string(lib_math::round(pi, 2));
        std::cout << '\n' << std::endl;
    }

    // LINEAR INTERPOLATION
    {
        std::cout << "LINEAR INTERPOLATION" << std::endl;

        bool extrapolate = true;
        std::vector<double> x = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
        std::vector<double> y = {-2.30258509, -1.60943791, -1.20397280, -0.91629073, -0.69314718, -0.51082562, -0.35667494, -0.22314355, -0.10536052, 0.00000000,
                                  0.09531018,  0.18232156,  0.26236426,  0.33647224,  0.40546511,  0.47000363,  0.53062825,  0.58778666,  0.64185389, 0.69314718};
        std::vector<double> X = {0.09, 0.15, 0.25, 2.10};
        std::vector<double> Y = lib_math::lin_interp1d(x, y, X, extrapolate);

        for (std::size_t idx = 0; idx < X.size(); idx++)
        {
            std::cout << "exact:         ln(" << std::to_string(X[idx]) << ") =  " << std::to_string(std::log(X[idx])) << std::endl;
            std::cout << "interpolation: ln(" << std::to_string(X[idx]) << ") => " << std::to_string(Y[idx]) << std::endl;
        }

        std::cout << '\n' << std::endl;
    }

    // NORMAL DISTRIBUTION
    {
        std::cout << "NORMAL DISTRIBUTION" << std::endl;

        std::vector<double> x = {-4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0};
        std::vector<double> pdf = lib_math::norm_pdf(x);
        std::vector<double> cdf = lib_math::norm_cdf(x);

        for (int idx = 0; idx < x.size(); idx++)
        {
            std::cout << "x: " << std::to_string(x[idx]) << " -> pdf: " << std::to_string(pdf[idx]) << ", cdf: " << std::to_string(cdf[idx]) << std::endl;
        }

        std::vector<double> q = {0.001, 0.01, 0.10, 0.25, 0.50, 0.75, 0.90, 0.99, 0.999};
        std::vector<double> z = lib_math::norm_inv(q);

        for (int idx = 0; idx < q.size(); idx++)
        {
            std::cout << "q: " << std::to_string(q[idx]) << " -> inv: " << std::to_string(z[idx])<< std::endl;
        }
        std::cout << '\n' << std::endl;
    }

    // NEWTON-RAPHSON METHOD
    {
        std::cout << "NEWTON-RAPHSON METHOD" << std::endl;

        double X = 1.5;  // initial estimate of square root of 2
        double step = 1e-5;
        double tolerance = 1e-7;
        int iter_max = 1000;

        lib_math::newton_raphson_res res = lib_math::newton_raphson(sqrt_of_2, X, step, tolerance, iter_max);

        if (res.success)
            std::cout << "Newton-Raphson method: success" << std::endl;
        else
            std::cout << "Newton-Raphson method: failure" << std::endl;

        std::cout << "square root of 2 is " << std::to_string(res.x) << std::endl;
        std::cout << "step: " << std::to_string(res.step) << "; tolerance: " << std::to_string(res.tolerance) << "; iter_no: " << std::to_string(res.iter_no) << "; iter_max: " << std::to_string(res.iter_max) << std::endl;
        std::cout << "msg: " << res.msg << std::endl;
        std::cout << '\n' << std::endl;
    }

    // EMPIRICAL PROBABILITY DISTRIBUTION
    {
        std::cout << "EMPIRICAL PROBABILITY DISTRIBUTION" << std::endl;

        // cumulative normal distribution
        lib_math::myDiscProbDist cdf = lib_math::myDiscProbDist("include/my_libs/math/data/lib_math/norm_cdf.csv");
        std::cout << "norm_cdf(-0.25) = " << cdf.get(-0.25) << std::endl;

        // inverse normal distribution
        lib_math::myDiscProbDist inv = lib_math::myDiscProbDist("include/my_libs/math/data/lib_math/norm_inv.csv");
        std::vector<double> x = {0.01, 0.10, 0.50, 0.90, 0.95, 0.99};
        std::vector<double> y = inv.get(x);
        for (int i = 0; i < x.size(); i++)
            std::cout << "norm_inv(" << x[i] << ") = " << y[i] << std::endl;
        std::cout << '\n' << std::endl;
    }

    return 0;
}