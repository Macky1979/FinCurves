/** \example lib_df_examples.h
 * @file lib_df.h
 * @author Michal Mackanic
 * @brief Very simple dataframe which is used to load / retreive data into / from SQLite.
 * @version 1.0
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace lib_df
{
    /**
     * @brief User defined datatype to hold result of SQL query.
     * 
     */
    struct dataFrame
    {
        std::vector<std::string> col_nms;
        std::vector<std::string> dtypes;
        std::vector<std::vector<std::string>> values; 
    };

    /**
     * @brief Dataframe object.
     * 
     */
    class myDataFrame
    {
        private:
        
        public:
            /**
             * @brief Data structure describing dataframe.
             * 
             */
            dataFrame tbl;

            /**
             * @brief Construct a new myDataFrame object.
             * 
             */
            myDataFrame(){};

            /**
             * @brief Construct a new myDataFrame object
             * 
             * @param tbl Data structure with table data.
             */
            myDataFrame(dataFrame tbl){this->tbl = tbl;};

            /**
             * @brief Destroy the myDataFrame object.
             * 
             */
            ~myDataFrame(){};

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