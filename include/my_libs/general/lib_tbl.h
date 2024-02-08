/** \example lib_tbl_examples.h
 * @file lib_tbl.h
 * @author Michal Mackanic
 * @brief 
 * @version 1.0
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>

namespace lib_tbl
{
    // 
    /**
     * @brief User defined datatype to hold table content.
     * 
     */
    struct data
    {
        std::vector<std::string> col_nms;
        std::vector<std::vector<std::string>> values; 
    };

    /**
     * @brief Dataframe object.
     * 
     */
    class myTable
    {
        private:
            std::list<std::string> true_list = {"Y", "y", "YES", "Yes", "yes", "1", "True", "true"};
            std::list<std::string> missing_list = {"NULL", "null", "N/A", "n/a", "#N/A", "#n/a", "", " "};

        public:
            /**
             * @brief Table data.
             * 
             */
            data tbl;

            /**
             * @brief Construct a new myTable object.
             * 
             */
            myTable(){};

            /**
             * @brief Construct a new myDataFrame object
             * 
             * @param tbl Data structure with table data.
             */
            myTable(data tbl){this->tbl = tbl;};

            /**
             * @brief Destroy the myTable object.
             * 
             */
            ~myTable(){};

            /**
             * @brief Deep copy of myTable object.
             * 
             * @param source Source myTable object.
             */
            void copy(const myTable& source);

            /**
             * @brief Deep copy of myTable object.
             * 
             * @param source Shared pointer to source myTable object.
             */
            void copy(const std::shared_ptr<lib_tbl::myTable> source);

            /**
             * @brief Clear all data.
             * 
             */
            void clear();

            /**
             * @brief Get number of rows.
             * 
             * @return std::size_t Number of rows.
             */
            std::size_t get_rows_no() const;

            /**
             * @brief Get number of columns.
             * 
             * @return std::size_t Number of columns.
             */
            std::size_t get_cols_no() const;

            /**
             * @brief Get rows indices for a given column
             * 
             * @param col_nm Column name.
             * @param type 0 => All rows in specified range. \n 1 => Skip rows with N/A values. \n 2 => Return row IDs with N/A values.
             * @param first_row_idx Number of the first row to extract. Value -1 is interpreted as the first row.
             * @param last_row_idx Number of the last row to extract. Value -1 is interpreted as the last row.
             * @return std::vector<std::shared_ptr<std::size_t>> Vector with row IDs.
             */
            std::vector<std::size_t> get_rows_idx(const std::string& col_nm, const unsigned int type = 0, const std::size_t first_row_idx = -1, const std::size_t last_row_idx = -1);

            /**
             * @brief Get string type column.
             * 
             * @param col_nm Column name.
             * @param first_row_idx Number of the first row to extract. Value -1 is interpreted as the first row.
             * @param last_row_idx Number of the last row to extract. Value -1 is interpreted as the last row.
             * @return std::vector<std::string> Column values represented as vector of strings.
             */
            std::vector<std::shared_ptr<std::string>> get_str_column(const std::string& col_nm, const bool exclude_na = false, const std::size_t first_row_idx = -1, const std::size_t last_row_idx = -1);

            /**
             * @brief Get string type column.
             * 
             * @param col_nm Column name.
             * @param rows_idx Vector of row IDs.
             * @return std::vector<std::shared_ptr<std::string>> Column values represented as vector of strings.
             */
            std::vector<std::shared_ptr<std::string>> get_str_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx);

            /**
             * @brief Get integer type column.
             * 
             * @param col_nm Column name.
             * @param exclude_na Exclude N/A values.
             * @param first_row_idx Number of the first row to extract. Value -1 is interpreted as the first row.
             * @param last_row_idx Number of the last row to extract. Value -1 is interpreted as the last row.
             * @return std::vector<int> Column values represented as vector of integers.
             */
            std::vector<std::shared_ptr<int>> get_int_column(const std::string& col_nm, const bool exclude_na = false, const std::size_t first_row_idx = -1, const std::size_t last_row_idx = -1);

            /**
             * @brief Get integer type column.
             * 
             * @param col_nm Column name.
             * @param rows_idx Vector of row IDs.
             * @return std::vector<std::shared_ptr<int>> Column values represented as vector of integers.
             */
            std::vector<std::shared_ptr<int>> get_int_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx);

            /**
             * @brief Get long integer type column.
             * 
             * @param col_nm Column name.
             * @param exclude_na Exclude N/A values.
             * @param first_row_idx Number of the first row to extract. Value -1 is interpreted as the first row.
             * @param last_row_idx Number of the last row to extract. Value -1 is interpreted as the last row.
             * @return std::vector<long> Column values represented as vector of long integers.
             */
            std::vector<std::shared_ptr<long>> get_long_column(const std::string& col_nm, const bool exclude_na = false, const std::size_t first_row_idx = -1, const std::size_t last_row_idx = -1);

            /**
             * @brief Get long integer type column.
             * 
             * @param col_nm Column name.
             * @param rows_idx Vector of row IDs.
             * @return std::vector<std::shared_ptr<int>> Column values represented as vector of long integers.
             */
            std::vector<std::shared_ptr<long>> get_long_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx);

            /**
             * @brief Get double type column.
             * 
             * @param col_nm Column name.
             * @param exclude_na Exclude N/A values.
             * @param first_row_idx Number of the first row to extract. Value -1 is interpreted as the first row.
             * @param last_row_idx Number of the last row to extract. Value -1 is interpreted as the last row.
             * @return std::vector<double> Column values represented as vector of doubles.
             */
            std::vector<std::shared_ptr<double>> get_double_column(const std::string& col_nm, const bool exclude_na = false, const std::size_t first_row_idx = -1, const std::size_t last_row_idx = -1);

            /**
             * @brief Get double type column.
             * 
             * @param col_nm Column name.
             * @param rows_idx Vector of row IDs.
             * @return std::vector<std::shared_ptr<double>> Column values represented as vector of doubles.
             */
            std::vector<std::shared_ptr<double>> get_double_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx);

            /**
             * @brief Get boolean type column.
             * 
             * @param col_nm Column name.
             * @param exclude_na Exclude N/A values.
             * @param first_row_idx Number of the first row to extract. Value -1 is interpreted as the first row.
             * @param last_row_idx Number of the last row to extract. Value -1 is interpreted as the last row.
             * @return std::vector<double> Column values represented as vector of booleans.
             */
            std::vector<std::shared_ptr<bool>> get_bool_column(const std::string& col_nm, const bool exclude_na = false, const std::size_t first_row_idx = -1, const std::size_t last_row_idx = -1);

            /**
             * @brief Get boolean type column.
             * 
             * @param col_nm Column name.
             * @param rows_idx Vector of row IDs.
             * @return std::vector<std::shared_ptr<bool>> Column values represented as vector of booleans.
             */
            std::vector<std::shared_ptr<bool>> get_bool_column(const std::string& col_nm, const std::vector<std::size_t>& rows_idx);

            /**
             * @brief Print dataframe on screen.
             * 
             * @param rows_no Number of rows to be printed.
             */
            void print(const std::size_t& rows_no = 10);

            /**
             * @brief Store dataframe in a .csv file.
             * 
             * @param file_nm File name.
             * @param sep Column separator.
             * @param quotes Quotes to be used for text strings.
             */
            void write(const std::string& file_nm, const std::string& sep, const bool& quotes) const;

            /**
             * @brief Read dataframe from a .csv file.
             * 
             * @param file_nm File name.
             * @param sep Columns separator.
             * @param quotes Quotest to be used for text strings.
             */
            void read(const std::string& file_nm, const std::string& sep, const bool& quotes);
    };
}