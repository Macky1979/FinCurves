#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include "lib_str.h"
#include "lib_df.h"

namespace lib_df
{
    /*
     * OBJECT FUNCTIONS
     */

    // clear content of the dataframe
    void lib_df::myDataFrame::clear()
    {
        this->tbl.col_nms.clear();
        this->tbl.dtypes.clear();
        this->tbl.values.clear();
    }

    // return number of dataframe rows
    std::size_t lib_df::myDataFrame::get_rows_no() const
    {
        return this->tbl.values.size();
    }

    // return number of dataframe columns
    std::size_t lib_df::myDataFrame::get_cols_no() const
    {
        return this->tbl.col_nms.size();
    }

    // auxiliary function that prepares a single row to be written into a text file
    std::string process_row(const std::vector<std::string>& cols, const std::string& sep, const bool& quotes)
    {
        // std::string to hold the row
        std::string row = "";

        // go column by column
        for (std::size_t col_idx = 0; col_idx < cols.size(); col_idx++)
        {
            // check if each column should be "closed" within quotes
            if (quotes)
                row += "\"" + cols[col_idx] + "\"";
            else
                row += cols[col_idx];

            // add column separator
            row += sep;
        }

        // remove quotes and sepator at the end of the last column
        if (quotes)
        {
            row = row.substr(0, row.size() - 2);
            row += "\"";
        }
        else
            row = row.substr(0, row.size() - 1);

        // add end-of-line character
        row += '\n';

        // return row
        return row;
    }

    // print dataframe on screen
    void lib_df::myDataFrame::print(const std::size_t& rows_no)
    {
        // wide of individual columns
        std::vector<unsigned short> cols_wide;

        // get number of rows
        std::size_t rows_no_aux = std::min(rows_no, this->get_rows_no());

        // find column width
        for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
        {
            std::size_t col_wide = this->tbl.col_nms[col_idx].size();
            for (std::size_t row_idx = 0; row_idx < rows_no_aux; row_idx++)
                col_wide = std::max(col_wide, this->tbl.values[row_idx][col_idx].size());
            cols_wide.push_back(col_wide);
        }

        // inform about number of rows being print

        // table start
        {
            std::string row = "=";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char("=", cols_wide[col_idx], '=') + '=';
            std::cout << row << std::endl;        
        }

        // print column names
        {
            std::string row = "|";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char(this->tbl.col_nms[col_idx], cols_wide[col_idx], ' ') + '|';
            std::cout << row << std::endl;
        }

        // separate column names from data types
        {
            std::string row = "-";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char("-", cols_wide[col_idx], '-') + '-';
            std::cout << row << std::endl;        
        }

        // print date types
        {
            std::string row = "|";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char(this->tbl.dtypes[col_idx], cols_wide[col_idx], ' ') + '|';
            std::cout << row << std::endl;
        }

        // separate data types from data 
        {
            std::string row = "=";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char("=", cols_wide[col_idx], '=') + '=';
            std::cout << row << std::endl;        
        }

        // print the data
        {
            for (std::size_t row_idx = 0; row_idx < rows_no_aux; row_idx++)
            {
                std::string row = "|";
                for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                    row += lib_str::add_leading_char(this->tbl.values[row_idx][col_idx], cols_wide[col_idx], ' ') + '|';
                std::cout << row << std::endl;
            }

        }

        // table end
        {
            std::string row = "=";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char("=", cols_wide[col_idx], '=') + '=';
            std::cout << row << std::endl;        
        }

    }

    // write dataframe into a .csv file
    void lib_df::myDataFrame::write(const std::string& file_nm, const std::string& sep, const bool& quotes) const
    {
        // variable to hold row
        std::string row;

        // open file
        std::ofstream f;
        f.open(file_nm);

        // check that the file is indeed opened
        if (f.is_open())
        {
            // write column names
            row = process_row(this->tbl.col_nms, sep, quotes);
            f << row;

            // write data types
            row = process_row(this->tbl.dtypes, sep, quotes);
            f << row;   

            // go row by row
            for (long row_idx = 0; row_idx < tbl.values.size(); row_idx++)
            {
                row = process_row(this->tbl.values[row_idx], sep, quotes);
                f << row;
            }

            // close file
            f.close();
        }
        else
            throw std::runtime_error((std::string)__func__ + ": Unable to open file " + file_nm + "!");
    }

    // cut row read from .csv file into vector of cells
    std::shared_ptr<std::vector<std::string>> cut_row_into_cells(std::string& row, const std::string& sep, const bool& quotes)
    {
        // column separator position
        std::size_t pos;

        // variable to hold vector of cells
        std::shared_ptr<std::vector<std::string>> cells = std::make_shared<std::vector<std::string>>();

        // separator
        std::string sep_aux = sep;
        if (quotes)
            sep_aux = "\"" + sep_aux + "\"";

        // get rid of the leading and trailing quote
        if (quotes)
        {
            row = row.substr(1, row.size() - 1);
            row = row.substr(0, row.size() - 1);
        }

        // cut row into cells
        while (row.find(sep_aux) != std::string::npos)
        {
            // get position of the column separator
            pos = row.find(sep_aux);

            // cut a cell
            cells->push_back(row.substr(0, pos));

            // remove the cell from row
            row = row.substr(pos + sep_aux.size(), row.size() - pos - sep_aux.size());
        }
        cells->push_back(row);

        // return cells
        return cells;
    }

    // read data from a .csv file
    void lib_df::myDataFrame::read(const std::string& file_nm, const std::string& sep, const bool& quotes)
    {
        // row read from the .csv file
        std::string row;

        // vector of cells
        std::shared_ptr<std::vector<std::string>> cells;

        // open file
        std::ifstream f;
        f.open(file_nm, std::ios::out);

        // check that the file is indeed opened
        if (f.is_open())
        {
            // read columns names
            getline(f, row);
            cells = cut_row_into_cells(row, sep, quotes);
            this->tbl.col_nms = *cells;

            // read column types
            getline(f, row);
            cells = cut_row_into_cells(row, sep, quotes);
            this->tbl.dtypes = *cells;

            // read the values
            while (getline(f, row))
            {
                cells = cut_row_into_cells(row, sep, quotes);
                this->tbl.values.push_back(*cells);           
            }

            // close file
            f.close();
        }
        else
            throw std::runtime_error((std::string)__func__ + ": Unable to open file " + file_nm + "!");
    }
}
