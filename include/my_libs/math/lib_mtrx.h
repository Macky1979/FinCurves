/** \example lib_mtrx_examples.h
 * @file lib_mtrx.h
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
#include <memory>

namespace lib_mtrx
{
    /**
     * @brief 2D matrix object.
     * 
     */
    class myMatrix2D
    {
        private:

        public:

            // object variables
            std::vector<std::vector<double>> elements;

            /**
             * @brief Construct a new x-by-y myMatrix2D object representing 2D matrix.
             * 
             * @param x Number of rows.
             * @param y Number of columns.
             * @param element Element to be used to fill in the matrix.
             * @param diagonal true => Diagonal migration matrix with elements on the main diagonal only and zeros elsewhere. Applicable only to square matrices. \n
             * false => Migration matrix is fully filled with the user specified element.
             */
            myMatrix2D(const unsigned short& x, const unsigned short& y, const double& element = 0.0, const bool& diagonal = false);

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix from vector of vectors of doubles.
             * 
             * @param elements Vector of vectors of doubles.
             */
            myMatrix2D(const std::vector<std::vector<double>> elements);

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix based on matrix stored in a .csv file.
             * 
             * @param file_nm Name of the .csv file holding matrix.
             * @param sep Column separator used in the .csv file.
             */
            myMatrix2D(const std::string& file_nm, const std::string& sep = ",");

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix based on another myMatrix2D object.
             * 
             * @param mtrx Existing myMatrix2D object which is used to create a new myMatrix2D object.
             */
            myMatrix2D(const myMatrix2D& mtrx);

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix from a range of another myMatrix2D object.
             * 
             * @param mtrx Existing myMatrix2D object which is used to create a new myMatrix2D object.
             * @param rng Range in form of {x_low,x_high,y_low,y_high} specifying what part of the existing myMatrix2D is be used to create a new myMatrix2D object.
             */
            myMatrix2D(const myMatrix2D& mtrx, const std::vector<unsigned short>& rng);

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix from a range of another myMatrix2D object.
             * 
             * @param mtrx Existing myMatrix2D object which is used to create a new myMatrix2D object.
             * @param rng Range in form of "x_low:x_high,y_low:y_high" specifying what part of the existing myMatrix2D is be used to create a new myMatrix2D object.
             */
            myMatrix2D(const myMatrix2D& mtrx, const std::string& rng);

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix based on another myMatrix2D object.
             * 
             * @param mtrx Existing myMatrix2D object which is used to create a new myMatrix2D object.
             */
            myMatrix2D(const std::shared_ptr<myMatrix2D> mtrx);

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix from a range of another myMatrix2D object.
             * 
             * @param mtrx Existing myMatrix2D object which is used to create a new myMatrix2D object.
             * @param rng Range in form of {x_low,x_high,y_low,y_high} specifying what part of the existing myMatrix2D is be used to create a new myMatrix2D object.
             */
            myMatrix2D(const std::shared_ptr<myMatrix2D> mtrx, const std::vector<unsigned short>& rng);

            /**
             * @brief Construct a new myMatrix2D object representing 2D matrix from a range of another myMatrix2D object.
             * 
             * @param mtrx Existing myMatrix2D object which is used to create a new myMatrix2D object.
             * @param rng Range in form of "x_low:x_high,y_low:y_high" specifying what part of the existing myMatrix2D is be used to create a new myMatrix2D object.
             */
            myMatrix2D(const std::shared_ptr<myMatrix2D> mtrx, const std::string& rng);

            /**
             * @brief Destroy the myMatrix2D object.
             * 
             */
            ~myMatrix2D(){};

            /**
             * @brief Print matrix on screen.
             * 
             * @param precision Number of decimal digits to be used.
             */
            void print(const unsigned short& precision = 7) const;

            /**
             * @brief Save matrix into a .csv file.
             * 
             * @param file_nm Name of the .csv file.
             * @param sep Column separator used in .csv file.
             */
            void save(const std::string& file_nm, const char& sep = ',');

            /**
             * @brief Get number of matrix rows.
             * 
             * @return unsigned short Number of matrix rows.
             */
            unsigned short get_x() const {return this->elements.size();};

            /**
             * @brief Get number of matrix columns.
             * 
             * @return unsigned short Number of matrix columns.
             */
            unsigned short get_y() const {return this->elements[0].size();};

            /**
             * @brief Transpose matrix.
             * 
             * @return std::shared_ptr<myMatrix2D> Transponsed matrix.
             */
            std::shared_ptr<myMatrix2D> transpose();

            /**
             * @brief Sum matrix elements specified by a range defined through x_low, x_high, y_low and y_high.
             * 
             * @param x_low Lower row index.
             * @param x_high Upper row index.
             * @param y_low Lower column index.
             * @param y_high Upper column index.
             * @return float Sum of the elements.
             */
            float sum(const unsigned short& x_low, const unsigned short& x_high, const unsigned short& y_low, const unsigned short& y_high);

            /**
             * @brief Sum matrix elements specified by a range in form of {x_low,x_high,y_low,y_high}.
             * 
             * @param rng Range in form of {x_low,x_high,y_low,y_high}.
             * @return float Sum of the elements.
             */
            float sum(const std::vector<unsigned short>& rng = std::vector<unsigned short>());

            /**
             * @brief Sum matrix elements specified by a range in form of "x_low:x_high,y_low:y_high".
             * 
             * @param rng Range in form of "x_low:x_high,y_low:y_high".
             * @return float Sum of the elements.
             */
            float sum(const std::string& rng = "");

            /**
             * @brief Extract row from matrix.
             * 
             * @param x Position of row to be extracted from matrix.
             * @return std::shared_ptr<myMatrix2D> Extracted row.
             */
            std::shared_ptr<myMatrix2D> get_row(const unsigned short& x);

            /**
             * @brief Extract col from matrix.
             * 
             * @param y Position of column to be extracted from matrix.
             * @return std::shared_ptr<myMatrix2D> Extracted column.
             */
            std::shared_ptr<myMatrix2D> get_col(const unsigned short& y);

            /**
             * @brief Replace row in matrix.
             * 
             * @param vector Replacement row.
             * @param x Position of row to be replaced.
             */
            void replace_row(const std::shared_ptr<myMatrix2D> vector, const unsigned short& x);

            /**
             * @brief Replace column in matrix.
             * 
             * @param vector Replacement column.
             * @param y Position of column to be replaced.
             */
            void replace_col(const std::shared_ptr<myMatrix2D> vector, const unsigned short& y);

    };

    /**
     * @brief Add scalar to matrix.
     * 
     * @param mtrx Matrix to which the scalar should be added.
     * @param scalar_to_add Scalar to be added to matrix.
     * @return std::shared_ptr<myMatrix2D> Resulting matrix.
     */
    std::shared_ptr<myMatrix2D> add_scalar(const std::shared_ptr<myMatrix2D> mtrx, const double& scalar_to_add);

    /**
     * @brief Sum up matrices.
     * 
     * @param mtrxs Vector of matrices to be summed up.
     * @return std::shared_ptr<myMatrix2D> Resulting matrix.
     */
    std::shared_ptr<myMatrix2D> add_mtrx(const std::vector<std::shared_ptr<myMatrix2D>> mtrxs);

    /**
     * @brief Multiply matrix with a scalar.
     * 
     * @param mtrx Matrix to be multiplied with a scalar.
     * @param scalar_to_mult Scalar to multiply matrix with.
     * @return std::shared_ptr<myMatrix2D> Resulting matrix.
     */
    std::shared_ptr<myMatrix2D> mult_scalar(const std::shared_ptr<myMatrix2D> mtrx, const double& scalar_to_mult);

    /**
     * @brief Multiply through matrices.
     * 
     * @param mtrxs Vector of matrices to be multiplied through.
     * @return std::shared_ptr<myMatrix2D> Resulting matrix.
     */
    std::shared_ptr<myMatrix2D> mult_mtrx(const std::vector<std::shared_ptr<myMatrix2D>> mtrxs);
}
