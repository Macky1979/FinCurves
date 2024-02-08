#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include "lib_str.h"
#include "lib_tbl.h"

namespace lib_tbl
{
    /*
     * OBJECT FUNCTIONS
     */

    // copy table
    void lib_tbl::myTable::copy(const std::shared_ptr<lib_tbl::myTable> source)
    {
        this->tbl.col_nms.clear();
        this->tbl.col_nms = source->tbl.col_nms;
        this->tbl.values.clear();
        this->tbl.values = source->tbl.values;
    }

    void lib_tbl::myTable::copy(const myTable& source)
    {
        this->tbl.col_nms.clear();
        this->tbl.col_nms = source.tbl.col_nms;
        this->tbl.values.clear();
        this->tbl.values = source.tbl.values;
    }

    // clear table content
    void lib_tbl::myTable::clear()
    {
        this->tbl.col_nms.clear();
        this->tbl.values.clear();
    }

    // return number of table rows
    std::size_t lib_tbl::myTable::get_rows_no() const
    {
        return this->tbl.values.size();
    }

    // return number of table columns
    std::size_t lib_tbl::myTable::get_cols_no() const
    {
        return this->tbl.col_nms.size();
    }

    // convert column name into column number
    int col_nm_to_idx(const std::vector<std::string>& col_nms, const std::string& col_nm)
    {

        std::size_t col_idx;
        for (std::size_t idx = 0; idx < col_nms.size(); idx++)
        {
            if (col_nms[idx].compare(col_nm) == 0)
                return idx;
        }
        throw std::runtime_error((std::string)__func__ + ": Column '" + col_nm + "' does not exist!");;
    }

    // determine the first and the last row to extract
    std::tuple<int, int> get_first_last_row_idx_idx(const int& rows_no, const int& first_row_idx, const int& last_row_idx)
    {
        // determine the first and the last row to extract
        std::size_t first_row_idx_;
        if (first_row_idx == -1)
            first_row_idx_ = 1;
        else
            first_row_idx_ = first_row_idx;

        std::size_t last_row_idx_;
        if (last_row_idx == -1)
            last_row_idx_ = rows_no;
        else
            last_row_idx_ = last_row_idx;

        // check the first and the last row
        if (first_row_idx > last_row_idx)
            throw std::runtime_error((std::string)__func__ + ": The first row is larger than the last row!");

        // return row IDs
        return std::tuple<std::size_t, std::size_t>(first_row_idx_, last_row_idx_);
    }

    // get row IDs
    std::vector<std::size_t> lib_tbl::myTable::get_rows_idx(const std::string& col_nm, const unsigned int type, const std::size_t first_row_idx, const std::size_t last_row_idx)
    {
        // convert column name into column number
        std::size_t col_idx = col_nm_to_idx(this->tbl.col_nms, col_nm);

        // determine the first and the last row to extract
        std::tuple<std::size_t, std::size_t> first_last_row_idx = get_first_last_row_idx_idx(this->tbl.values.size(), first_row_idx, last_row_idx);
        std::size_t first_row_idx_ = std::get<0>(first_last_row_idx);
        std::size_t last_row_idx_ = std::get<1>(first_last_row_idx);

        // extract column
        bool is_missing;
        std::vector<std::size_t> column;
        for (std::size_t row_idx = first_row_idx_; row_idx <= last_row_idx_; row_idx++)
        {
            // all rows
            if (type == 0)
                column.push_back(row_idx);

            else
            {
                std::string aux = this->tbl.values[row_idx - 1][col_idx];
                is_missing = (std::find(this->missing_list.begin(), this->missing_list.end(), aux) != this->missing_list.end());

                // exclude N/A values
                if ((type == 1) && (!is_missing))
                    column.push_back(row_idx);

                // only N/A values
                if ((type == 2) && (is_missing))
                    column.push_back(row_idx);
            }
        }

        // return column
        return column;
    }

    // extract string column
    std::vector<std::shared_ptr<std::string>> lib_tbl::myTable::get_str_column(const std::string& col_nm, const bool exclude_na, const std::size_t first_row_idx, const std::size_t last_row_idx)
    {
        // convert column name into column number
        std::size_t col_idx = col_nm_to_idx(this->tbl.col_nms, col_nm);

        // determine the first and the last row to extract
        std::tuple<std::size_t, std::size_t> first_last_row_idx = get_first_last_row_idx_idx(this->tbl.values.size(), first_row_idx, last_row_idx);
        std::size_t first_row_idx_ = std::get<0>(first_last_row_idx);
        std::size_t last_row_idx_ = std::get<1>(first_last_row_idx);

        // extract column
        bool is_missing;
        std::vector<std::shared_ptr<std::string>> column;
        for (std::size_t row_idx = first_row_idx_; row_idx <= last_row_idx_; row_idx++)
        {
            std::string aux = this->tbl.values[row_idx - 1][col_idx];
            is_missing = (std::find(this->missing_list.begin(), this->missing_list.end(), aux) != this->missing_list.end());
            if ((is_missing) and !exclude_na)
                column.push_back(nullptr);
            else
                column.push_back(std::make_shared<std::string>(aux));
        };

        // return column
        return column;
    }

    std::vector<std::shared_ptr<std::string>> lib_tbl::myTable::get_str_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx)
    {
        // convert column name into column number
        std::size_t col_idx = col_nm_to_idx(this->tbl.col_nms, col_nm);

        // extract column
        bool is_missing;
        std::vector<std::shared_ptr<std::string>> column;
        for (std::size_t row_idx : rows_idx)
        {
            std::string aux = this->tbl.values[row_idx - 1][col_idx];
            is_missing = (std::find(this->missing_list.begin(), this->missing_list.end(), aux) != this->missing_list.end());
            if (is_missing)
                column.push_back(nullptr);
            else
                column.push_back(std::make_shared<std::string>(aux));
        }

        // return column
        return column;
    }

    // extract integer column
    std::vector<std::shared_ptr<int>> lib_tbl::myTable::get_int_column(const std::string& col_nm, const bool exclude_na, const std::size_t first_row_idx, const std::size_t last_row_idx)
    {
        std::vector<std::shared_ptr<int>> int_column;
        std::vector<std::shared_ptr<std::string>> str_column = lib_tbl::myTable::get_str_column(col_nm, exclude_na, first_row_idx, last_row_idx);
        for (std::size_t idx = 0; idx < str_column.size(); idx++)
        {
            if (str_column[idx] == nullptr)
                int_column.push_back(nullptr);
            else
            {
                int aux = std::stoi(*str_column[idx]);
                int_column.push_back(std::make_shared<int>(aux));
            }
        }
        return int_column;
    }

    std::vector<std::shared_ptr<int>> lib_tbl::myTable::get_int_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx)
    {
        // convert column name into column number
        std::size_t col_idx = col_nm_to_idx(this->tbl.col_nms, col_nm);

        // extract column
        bool is_missing;
        std::vector<std::shared_ptr<int>> column;
        for (std::size_t row_idx : rows_idx)
        {
            std::string aux = this->tbl.values[row_idx - 1][col_idx];
            is_missing = (std::find(this->missing_list.begin(), this->missing_list.end(), aux) != this->missing_list.end());
            if (is_missing)
                column.push_back(nullptr);
            else
                column.push_back(std::make_shared<int>(std::stoi(aux)));
        }

        // return column
        return column;
    }

    // extract long column
    std::vector<std::shared_ptr<long>> lib_tbl::myTable::get_long_column(const std::string& col_nm, const bool exclude_na, const std::size_t first_row_idx, const std::size_t last_row_idx)
    {
        std::vector<std::shared_ptr<long>> long_column;
        std::vector<std::shared_ptr<std::string>> str_column = lib_tbl::myTable::get_str_column(col_nm, exclude_na, first_row_idx, last_row_idx);
        for (std::size_t idx = 0; idx < str_column.size(); idx++)
        {
            if (str_column[idx] == nullptr)
                long_column.push_back(nullptr);
            else
            {
                long aux = std::stol(*str_column[idx]);
                long_column.push_back(std::make_shared<long>(aux));
            }
        }
        return long_column;
    }

    std::vector<std::shared_ptr<long>> lib_tbl::myTable::get_long_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx)
    {
        // convert column name into column number
        std::size_t col_idx = col_nm_to_idx(this->tbl.col_nms, col_nm);

        // extract column
        bool is_missing;
        std::vector<std::shared_ptr<long>> column;
        for (std::size_t row_idx : rows_idx)
        {
            std::string aux = this->tbl.values[row_idx - 1][col_idx];
            is_missing = (std::find(this->missing_list.begin(), this->missing_list.end(), aux) != this->missing_list.end());
            if (is_missing)
                column.push_back(nullptr);
            else
                column.push_back(std::make_shared<long>(std::stol(aux)));
        }

        // return column
        return column;
    }

    // extract double column
    std::vector<std::shared_ptr<double>> lib_tbl::myTable::get_double_column(const std::string& col_nm, const bool exclude_na, const std::size_t first_row_idx, const std::size_t last_row_idx)
    {
        std::vector<std::shared_ptr<double>> double_column;
        std::vector<std::shared_ptr<std::string>> str_column = lib_tbl::myTable::get_str_column(col_nm, exclude_na, first_row_idx, last_row_idx);
        for (std::size_t idx = 0; idx < str_column.size(); idx++)
        {
            if (str_column[idx] == nullptr)
                double_column.push_back(nullptr);
            else
            {
                double aux = std::stod(*str_column[idx]);
                double_column.push_back(std::make_shared<double>(aux));
            }
        }
        return double_column;
    }

    std::vector<std::shared_ptr<double>> lib_tbl::myTable::get_double_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx)
    {
        // convert column name into column number
        std::size_t col_idx = col_nm_to_idx(this->tbl.col_nms, col_nm);

        // extract column
        bool is_missing;
        std::vector<std::shared_ptr<double>> column;
        for (std::size_t row_idx : rows_idx)
        {
            std::string aux = this->tbl.values[row_idx - 1][col_idx];
            is_missing = (std::find(this->missing_list.begin(), this->missing_list.end(), aux) != this->missing_list.end());
            if (is_missing)
                column.push_back(nullptr);
            else
                column.push_back(std::make_shared<double>(std::stod(aux)));
        }

        // return column
        return column;
    }

    // extract bool column
    std::vector<std::shared_ptr<bool>> lib_tbl::myTable::get_bool_column(const std::string& col_nm, const bool exclude_na, const std::size_t first_row_idx, const std::size_t last_row_idx)
    {
        bool is_true;
        std::vector<std::shared_ptr<bool>> bool_column;
        std::vector<std::shared_ptr<std::string>> str_column = lib_tbl::myTable::get_str_column(col_nm, exclude_na, first_row_idx, last_row_idx);

        for (std::size_t idx = 0; idx < str_column.size(); idx++)
        {
            if (str_column[idx] == nullptr)
                bool_column.push_back(nullptr);
            else
            {
                is_true = (std::find(this->true_list.begin(), this->true_list.end(), *str_column[idx]) != this->true_list.end());
                if (is_true)
                    bool_column.push_back(std::make_shared<bool>(true));
                else
                    bool_column.push_back(std::make_shared<bool>(false));
            }
        }
        return bool_column;
    }

    std::vector<std::shared_ptr<bool>> lib_tbl::myTable::get_bool_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx)
    {
        // convert column name into column number
        std::size_t col_idx = col_nm_to_idx(this->tbl.col_nms, col_nm);

        // extract column
        bool is_missing;
        bool is_true;
        std::vector<std::shared_ptr<bool>> column;
        for (std::size_t row_idx : rows_idx)
        {
            std::string aux = this->tbl.values[row_idx - 1][col_idx];
            is_missing = (std::find(this->missing_list.begin(), this->missing_list.end(), aux) != this->missing_list.end());
            if (is_missing)
                column.push_back(nullptr);
            else
                is_true = (std::find(this->true_list.begin(), this->true_list.end(), aux) != this->true_list.end());
                if (is_true)
                    column.push_back(std::make_shared<bool>(true));
                else
                    column.push_back(std::make_shared<bool>(true));
        }

        // return column
        return column;
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

    // print table on screen
    void lib_tbl::myTable::print(const std::size_t& rows_no)
    {
        // wide of individual columns
        std::vector<unsigned short> cols_width;

        // get number of rows
        std::size_t rows_no_aux = std::min(rows_no, this->get_rows_no());

        // find column width
        for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
        {
            std::size_t col_width = this->tbl.col_nms[col_idx].size();
            for (std::size_t row_idx = 0; row_idx < rows_no_aux; row_idx++)
                col_width = std::max(col_width, this->tbl.values[row_idx][col_idx].size());
            cols_width.push_back(col_width);
        }

        // inform about number of rows being print

        // table start
        {
            std::string row = "=";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char("=", cols_width[col_idx], '=') + '=';
            std::cout << row << std::endl;
        }

        // print column names
        {
            std::string row = "|";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char(this->tbl.col_nms[col_idx], cols_width[col_idx], ' ') + '|';
            std::cout << row << std::endl;
        }

        // separate column dates from data 
        {
            std::string row = "=";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char("=", cols_width[col_idx], '=') + '=';
            std::cout << row << std::endl;
        }

        // print the data
        {
            for (std::size_t row_idx = 0; row_idx < rows_no_aux; row_idx++)
            {
                std::string row = "|";
                for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                    row += lib_str::add_leading_char(this->tbl.values[row_idx][col_idx], cols_width[col_idx], ' ') + '|';
                std::cout << row << std::endl;
            }

        }

        // table end
        {
            std::string row = "=";
            for (std::size_t col_idx = 0; col_idx < this->get_cols_no(); col_idx++)
                row += lib_str::add_leading_char("=", cols_width[col_idx], '=') + '=';
            std::cout << row << std::endl;        
        }

    }

    // write table into a .csv file
    void lib_tbl::myTable::write(const std::string& file_nm, const std::string& sep, const bool& quotes) const
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
    void lib_tbl::myTable::read(const std::string& file_nm, const std::string& sep, const bool& quotes)
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
