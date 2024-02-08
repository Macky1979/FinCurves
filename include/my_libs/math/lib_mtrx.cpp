#include <string>
#include <iostream>
#include <memory>
#include <iostream>
#include <fstream>
#include <memory>
#include "lib_mtrx.h"
#include "lib_str.h"
#include "lib_math.h"

/*
 * AUXILIARY FUNCTIONS
 */

// process matrix range
std::vector<unsigned short> get_mtrx_rng(const std::string& rng, const unsigned short x, const unsigned short y)
{
    // array holding decomposed range    
    std::vector<unsigned short> rng_;

    // store position
    unsigned short pos;

    // split row and column information
    pos = rng.find(",");

    // get row and column information
    std::string row_info = rng.substr(0, pos);
    lib_str::remove_char(row_info);
    std::string col_info = rng.substr(pos + 1, rng.size());
    lib_str::remove_char(col_info);

    // decompose row information
    pos = row_info.find(":");
    std::string row_info_1 = row_info.substr(0, pos);
    std::string row_info_2 = row_info.substr(pos + 1, row_info.size());

    // decompose column information
    pos = col_info.find(":");
    std::string col_info_1 = col_info.substr(0, pos);
    std::string col_info_2 = col_info.substr(pos + 1, col_info.size());

    // process the first row information
    rng_.push_back(static_cast<unsigned short>(std::stoi(row_info_1)));
    if (rng_[0] < 0)
        rng_[0] = x + rng_[0] + 1;
    
    // process the second row information
    rng_.push_back(static_cast<unsigned short>(std::stoi(row_info_2)));
    if (rng_[1] < 0)
        rng_[1] = x + rng_[1] + 1;

    // check row information
    if (rng_[0] >= rng_[1])
        throw std::runtime_error((std::string)__func__ + ": " + rng + " does not specify row range correctly!");
    
    if ((rng_[0] > x) or (rng_[1] > x))
        throw std::runtime_error((std::string)__func__ + ": " + rng + " out or matrix range!");

    // process the first column information
    rng_.push_back(static_cast<unsigned short>(std::stoi(col_info_1)));
    if (rng_[2] < 0)
        rng_[2] = y + rng_[2] + 1;

    // process the second column information
    rng_.push_back(static_cast<unsigned short>(std::stoi(col_info_2)));
    if (rng_[3] < 0)
        rng_[3] = y + rng_[3] + 1;

    // check column information
    if (rng_[2] >= rng_[3])
        throw std::runtime_error((std::string)__func__ + ": " + rng + " does not specify column range correctly!");

    if ((rng_[2] > y) or (rng_[3] > y))
        throw std::runtime_error((std::string)__func__ + ": " + rng + " out or matrix range!");

    // return matrix ranges
    return rng_;
}

// multiply two matrices
std::shared_ptr<lib_mtrx::myMatrix2D> mult_mtrx_aux(const std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_1, const std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_2)
{
    // check matrix compatibility
    if (mtrx_1->get_y() != mtrx_2->get_x())
    {
        mtrx_1->print();
        mtrx_2->print();
        throw std::runtime_error((std::string)__func__ + ": Incompatible matrices!");
    }
    // create matrix to hold the resulting matrix
    std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_ = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_1->get_x(), mtrx_2->get_y(), 0.0);

    // multiply matrices => (m x n) * (n * k) = (m x k)
    for (unsigned short m = 0; m < mtrx_->get_x(); m++)
    {
        for (unsigned short k = 0; k < mtrx_->get_y(); k++)
        {
            for (unsigned short n = 0; n < mtrx_2->get_x(); n++)
                mtrx_->elements[m][k] += mtrx_1->elements[m][n] * mtrx_2->elements[n][k];
        }
    }

    // return the resulting matrix
    return mtrx_;
}

// convert row read from .csv file into vector of doubles
std::vector<double> row_into_doubles(std::string row, std::string sep)
{
    // column separator position
    size_t pos;

    // variable to hold vector of matrix elements read from a .csv file
    std::vector<double> elements_in_row;

    // cut row into columns
    while (row.find(sep) != std::string::npos)
    {
        // get position of the column separator
        pos = row.find(sep);

        // cut row into columns
        elements_in_row.push_back(std::stod(row.substr(0, pos)));

        // remove the column from row
        row = row.substr(pos + sep.size(), row.size() - pos - sep.size());
    }
    elements_in_row.push_back(std::stod(row));

    // return cells
    return elements_in_row;
}

/*
 * OBJECT CONSTRUCTORS
 */

// constructor
lib_mtrx::myMatrix2D::myMatrix2D(const unsigned short& x, const unsigned short& y, const double& element, const bool& diagonal)
{
    // in case of a diagonal matrix check that it is a square matrix
    if (diagonal && (x != y))
        throw std::runtime_error((std::string)__func__ + ": Diagonal matrix must be a square matrix!");

    // add matrix elements
    for (unsigned short x_ = 0; x_ < x; x_++)
    {
        std::vector<double> row;
        for (unsigned short y_ = 0; y_ < y; y_++)
        {
            if (diagonal && (x_ != y_))
                row.push_back(0.0);
            else
                row.push_back(element);
        }
        this->elements.push_back(row);
    }

}

// constructor
lib_mtrx::myMatrix2D::myMatrix2D(const std::vector<std::vector<double>> elements)
{
    // copy matrix elements
    this->elements = elements;
}

// constructor
lib_mtrx::myMatrix2D::myMatrix2D(const std::string& file_nm, const std::string& sep)
{
    // variables storing number of rows and columns
    unsigned short x = 0;
    unsigned short y = 0;    

    // matrix
    std::vector<std::vector<double>> elements;

    // open file
    std::ifstream f;
    f.open(file_nm, std::ios::out);

    // check that the file is indeed opened
    std::string row;
    if (f.is_open())
    {
        // read the values row by row
        while (getline(f, row))
        {
            // increate row counter;
            x++;

            // retrieve column values
            std::vector<double> elements_in_row = row_into_doubles(row, sep);
        
            // check column size consistency
            if (y != 0)
            {
                if (y != elements_in_row.size())
                    throw std::runtime_error((std::string)__func__ + ": Inconsitent number of columns in file " + file_nm + "!");
            }
            y = elements_in_row.size();
            elements.push_back(elements_in_row);
        }

        // close file
        f.close();
    
        // copy matrix elements
        this->elements = elements;
    }
    else
        throw std::runtime_error((std::string)__func__ + ": Unable to open file " + file_nm + "!");
}

// copy constructor
lib_mtrx::myMatrix2D::myMatrix2D(const myMatrix2D& mtrx)
{

    // copy matrix elements
    this->elements = mtrx.elements;
}

// copy constructor
lib_mtrx::myMatrix2D::myMatrix2D(const myMatrix2D& mtrx, const std::vector<unsigned short>& rng)
{
    // range variables
    unsigned short x_low;
    unsigned short x_high;
    unsigned short y_low;
    unsigned short y_high;

    // set ranges
    if (rng.size() == 0)
    {
        x_low = 0;
        x_high = mtrx.get_x();
        y_low = 0;
        y_high = mtrx.get_y();
    }
    else
    {
        x_low = rng[0];
        x_high = rng[1];
        y_low = rng[2];
        y_high = rng[3];
    }

    // initialize elements
    std::vector<std::vector<double>> elements;

    // copy individual matrix elements
    for (unsigned short x = x_low; x < x_high; x++)
    {
        std::vector<double> elements_in_row;
        for (unsigned short y = y_low; y < y_high; y++)
        {
            elements_in_row.push_back(mtrx.elements[x][y]);
        }
        elements.push_back(elements_in_row);
    }

    // copy matrix elements
    this->elements = elements;
}

// copy constructor
lib_mtrx::myMatrix2D::myMatrix2D(const myMatrix2D& mtrx, const std::string& rng)
{
    // convert range string into vector of integers
    std::vector<unsigned short> rng_ = get_mtrx_rng(rng, mtrx.get_x(), mtrx.get_y());

    //create a new matrix object
    myMatrix2D(mtrx, rng_);
}

// copy constructor
lib_mtrx::myMatrix2D::myMatrix2D(const std::shared_ptr<myMatrix2D> mtrx)
{
    // copy matrix elements
    this->elements = mtrx->elements;
}

// copy constructor
lib_mtrx::myMatrix2D::myMatrix2D(const std::shared_ptr<myMatrix2D> mtrx, const std::vector<unsigned short>& rng)
{
    // range variables
    unsigned short x_low;
    unsigned short x_high;
    unsigned short y_low;
    unsigned short y_high;

    // set ranges
    if (rng.size() == 0)
    {
        x_low = 0;
        x_high = mtrx->get_x();
        y_low = 0;
        y_high = mtrx->get_y();
    }
    else
    {
        x_low = rng[0];
        x_high = rng[1];
        y_low = rng[2];
        y_high = rng[3];
    }

    // initialize elements
    std::vector<std::vector<double>> elements;

    // copy individual matrix elements
    for (unsigned short x = x_low; x < x_high; x++)
    {
        std::vector<double> elements_in_row;
        for (unsigned short y = y_low; y < y_high; y++)
        {
            elements_in_row.push_back(mtrx->elements[x][y]);
        }
        elements.push_back(elements_in_row);
    }

    // copy matrix elements
    this->elements = elements;
}

// copy constructor
lib_mtrx::myMatrix2D::myMatrix2D(const std::shared_ptr<lib_mtrx::myMatrix2D> mtrx, const std::string& rng)
{
    // convert range string into vector of integers
    std::vector<unsigned short> rng_ = get_mtrx_rng(rng, mtrx->get_x(), mtrx->get_y());

    // create a new matrix object
    lib_mtrx::myMatrix2D(mtrx, rng_);
}

/*
 * OBJECT FUNCTIONS
 */

// print content of the matrix
void lib_mtrx::myMatrix2D::print(const unsigned short& precision) const
{
    std::string row;
    std::string mtrx = "[";

    for (unsigned short x = 0; x < this->get_x(); x++)
    {
        // prepare begining of the matrix row
        if (x == 0)
            row = "[";
        else
            row = " [";
        
        // store all elements from a single row
        for (unsigned short y = 0; y < this->get_y(); y++)
            row += std::to_string(lib_math::round(this->elements[x][y], precision)) + ", ";

        // end of the row
        row.erase(row.size() - 2);
        row += "]";

        if (x < this->get_x() - 1)
            row += '\n';

        // add row
        mtrx += row;
    }

    mtrx += "]";

    // print the matrix
    std::cout << mtrx << std::endl;
}

// save matrix into a file
void lib_mtrx::myMatrix2D::save(const std::string& file_nm, const char& sep)
{
    std::ofstream f;
    f.open(file_nm, std::ios::out);

    if (f.is_open())
    {
        // string to hold row
        std::string row = "";

        // go row by row
        for (unsigned short x = 0; x < this->get_x(); x++)
        {
            // go column by column
            row = "";
            for (unsigned short y = 0; y < this->get_y(); y++)
                row += std::to_string(this->elements[x][y]) + sep;
            
            // replace the last separator with end of line
            row[row.length() - 1] = '\n';

            // write the row into the file
            f << row;
        }

        f.close();
    }
    else
        throw std::runtime_error((std::string)__func__ + ": Unable to open file '" + file_nm + "'!");
}

// transpose matrix
std::shared_ptr<lib_mtrx::myMatrix2D> lib_mtrx::myMatrix2D::transpose()
{
    // auxiliary matrix which is used to hold transpose values
    std::vector<std::vector<double>> elements_aux;

    // initiate the auxiliary matrix 
    for (unsigned short y = 0; y < this->get_y(); y++)
    {
        std::vector<double> elements_in_row_aux(this->get_x());
        for (unsigned short x = 0; x < this->get_x(); x++)
            elements_in_row_aux[x] = 0.0;
        elements_aux.push_back(elements_in_row_aux);
    }

    // transpose values
    for (unsigned short x = 0; x < this->get_x(); x++)
    {
        for (unsigned short y = 0; y < this->get_y(); y++)
            elements_aux[y][x] = this->elements[x][y];
    }

    // create a shared pointer to a new migration matrix
    std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = std::make_shared<myMatrix2D>(elements_aux);

    // return the transposed matrix
    return mtrx;
}

// sum inner matrix elements
float lib_mtrx::myMatrix2D::sum(const unsigned short& x_low, const unsigned short& x_high, const unsigned short& y_low, const unsigned short& y_high)
{
    // deep copy individual matrix elements
    float sum = 0;
    for (unsigned short x = x_low; x < x_high; x++)
    {
        for (unsigned short y = y_low; y < y_high; y++)
            sum += this->elements[x][y];
    }

    // return the sum
    return sum;
}

// sum inner matrix elements
float lib_mtrx::myMatrix2D::sum(const std::vector<unsigned short>& rng)
{
    // range variables
    unsigned short x_low;
    unsigned short x_high;
    unsigned short y_low;
    unsigned short y_high;

    // convert range string into vector of integers
    if (rng.size() == 0)
    {
        x_low = 0;
        x_high = this->get_x();
        y_low = 0;
        y_high = this->get_y();  
    }
    else
    {
        x_low = rng[0];
        x_high = rng[1];
        y_low = rng[2];
        y_high = rng[3];     
    }

    // return sum
    return lib_mtrx::myMatrix2D::sum(x_low, x_high, y_low, y_high);

}

// sum inner matrix elements
float lib_mtrx::myMatrix2D::sum(const std::string& rng)
{
    // range variables
    unsigned short x_low;
    unsigned short x_high;
    unsigned short y_low;
    unsigned short y_high;

    // convert range string into vector of integers
    if (rng != "")
    {
        std::vector<unsigned short> rng_ = get_mtrx_rng(rng, this->get_x(), this->get_y());
        x_low = rng_[0];
        x_high = rng_[1];
        y_low = rng_[2];
        y_high = rng_[3];
    }
    else
    {
        x_low = 0;
        x_high = this->get_x();
        y_low = 0;
        y_high = this->get_y();
    }

    // return sum
    return myMatrix2D::sum(x_low, x_high, y_low, y_high);
}

// get row
std::shared_ptr<lib_mtrx::myMatrix2D> lib_mtrx::myMatrix2D::get_row(const unsigned short& x)
{
    // check row id
    if (x >= this->get_x())
    {
        this->print();
        throw std::runtime_error((std::string)__func__ + ": " + std::to_string(x) + " is invalid row number!");
    }

    // extract row
    unsigned short x_low = x;
    unsigned short x_high = x + 1;
    unsigned short y_low = 0;
    unsigned short y_high = this->get_y();
    std::vector<unsigned short> rng = {x_low, x_high, y_low, y_high};
    std::shared_ptr<lib_mtrx::myMatrix2D> row = std::make_shared<lib_mtrx::myMatrix2D>(*this, rng);
    return row;
}

// get column
std::shared_ptr<lib_mtrx::myMatrix2D> lib_mtrx::myMatrix2D::get_col(const unsigned short& y)
{
    // check column id
    if (y >= this->get_y())
    {
        this->print();
        throw std::runtime_error((std::string)__func__ + ": " + std::to_string(y) + " is invalid column number!");
    }

    // extract column
    unsigned short x_low = 0;
    unsigned short x_high = this->get_x();
    unsigned short y_low = y;
    unsigned short y_high = y + 1;
    std::vector<unsigned short> rng = {x_low, x_high, y_low, y_high};
    std::shared_ptr<lib_mtrx::myMatrix2D> col = std::make_shared<lib_mtrx::myMatrix2D>(*this, rng);
    return col;
}

// replace row
void lib_mtrx::myMatrix2D::replace_row(const std::shared_ptr<lib_mtrx::myMatrix2D> vector, const unsigned short& x)
{
    // check that the vector is a row vector
    if (vector->get_x() != 1)
    {
        vector->print();
        throw std::runtime_error((std::string)__func__ + ": Vector is not a row vector!");
    }

    // check that the vector and migration matrix are compatible
    if (this->get_y() != vector->get_y())
    {
        this->print();
        vector->print();
        throw std::runtime_error((std::string)__func__ + ": Vector is not compatible with the matrix!");
    }

    // check that the row to be replaced is OK
    if (x >= this->get_x())
    {
        this->print();
        throw std::runtime_error((std::string)__func__ + ": " + std::to_string(x) + " is not a valid row index!");
    }

    // replace the row
    for (unsigned short y = 0; y < this->get_y(); y++)
        this->elements[x][y] = vector->elements[0][y]; 
}

// replace column
void lib_mtrx::myMatrix2D::replace_col(const std::shared_ptr<lib_mtrx::myMatrix2D> vector, const unsigned short& y)
{
    // check that the vector is a column vector
    if (vector->get_y() != 1)
    {
        vector->print();
        throw std::runtime_error((std::string)__func__ + ": Vector is not a column vector!");
    }

    // check that the vector and migration matrix are compatible
    if (this->get_x() != vector->get_x())
    {
        this->print();
        vector->print();
        throw std::runtime_error((std::string)__func__ + ": Vector is not compatible with the matrix!");
    }

    // check that the column to be replaced is OK
    if (y >= this->get_y())
    {
        this->print();
        throw std::runtime_error((std::string)__func__ + ": " + std::to_string(y) + " is not a valid column index!");
    }

    // replace the column
    for (unsigned short x = 0; x < this->get_x(); x++)
        this->elements[x][y] = vector->elements[x][0]; 
}

/*
 * STAND-ALONE FUNCTIONS
 */

// add scalar to matrix
std::shared_ptr<lib_mtrx::myMatrix2D> lib_mtrx::add_scalar(const std::shared_ptr<lib_mtrx::myMatrix2D> mtrx, const double& scalar_to_add)
{
    // auxiliary matrix which is used to hold updated values
    std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_ = std::make_shared<lib_mtrx::myMatrix2D>(mtrx);

    // add the scalar
    for (unsigned short x = 0; x < mtrx_->get_x(); x++)
    {
        for (unsigned short y = 0; y < mtrx_->get_y(); y++)
            mtrx_->elements[x][y] += scalar_to_add;
    }

    // return updated matrix
    return mtrx;
}

// add multiple matrices
std::shared_ptr<lib_mtrx::myMatrix2D> lib_mtrx::add_mtrx(const std::vector<std::shared_ptr<lib_mtrx::myMatrix2D>> mtrxs)
{
    // create matrix to hold the resulting matrix
    std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_ = std::make_shared<lib_mtrx::myMatrix2D>(mtrxs[0]->get_x(), mtrxs[0]->get_x(), 0.0, false);

    // add the matrices
    for (std::shared_ptr<lib_mtrx::myMatrix2D> mtrx : mtrxs)
    {
        // check matrix compatibility
        if ((mtrx_->get_x() != mtrx->get_x()) or (mtrx_->get_y() != mtrx->get_y()))
            throw std::runtime_error((std::string)__func__ + ": Incompatible matrices!");

        // add matrices
        for (int x = 0; x < mtrx_->get_x(); x++)
        {
            for (int y = 0; y < mtrx_->get_y(); y++)
                mtrx_->elements[x][y] += mtrx->elements[x][y];
        }
    }
    
    // return the resulting matrix
    return mtrx_;  
}

// multiply matrix with scalar
std::shared_ptr<lib_mtrx::myMatrix2D> lib_mtrx::mult_scalar(const std::shared_ptr<lib_mtrx::myMatrix2D> mtrx, const double& scalar_to_mult)
{
    // create matrix to hold the resulting matrix
    std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_ = std::make_shared<lib_mtrx::myMatrix2D>(mtrx);

    for (unsigned short x = 0; x < mtrx_->get_x(); x++)
    {
        for (unsigned short y = 0; y < mtrx_->get_y(); y++)
            mtrx_->elements[x][y] *= scalar_to_mult;
    }

    // return the resulting matrix
    return mtrx_;   
}

// multiply multiple matrices
std::shared_ptr<lib_mtrx::myMatrix2D> lib_mtrx::mult_mtrx(const std::vector<std::shared_ptr<myMatrix2D>> mtrxs)
{
    // create matrix to hold the resulting matrix
    std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_ = std::make_shared<lib_mtrx::myMatrix2D>(mtrxs[0]);

    // multiply the matrices
    for (unsigned short idx = 1; idx < mtrxs.size(); idx++)
        mtrx_ = mult_mtrx_aux(mtrx_, mtrxs[idx]);

    // return the resulting matrix
    return mtrx_;  
}