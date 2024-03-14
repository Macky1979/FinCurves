/** \example lib_sql_examples.h
 * @file lib_sql.h
 * @author Michal Mackanic
 * @brief  User defined SQLite C++ functions.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include <memory>
#include "lib_df.h"

namespace lib_sql
{
    /**
     * @brief Read SQL query from a text file.
     * 
     * @param sql_file_nm Name of text file with SQL query.
     * @param tag One text file can contain multiple SQL queries. Tag is a string identifying begining of one particular SQL query. If an SQL query starts with ###!my_SQL, its tag is to be set to "my_SQL".
     * @return std::string SQL query retrieved from the text file.
     */
    std::string read_sql(const std::string& sql_file_nm, const std::string& tag);

    /**
     * @brief Make substitutions in SQL query retrieved from a text file.
     * 
     * @param sql SQL query retrieved from a text file.
     * @param replace_what String to be replaced, e.g. "###date###".
     * @param replace_with String to be used as a replacement, e.g. "30/12/1979".
     * @return std::string SQL query after the substitutions.
     */
    std::string replace_in_sql(std::string sql, std::string replace_what, std::string replace_with);

    /**
     * @brief SQLite object.
     * 
     */
    class mySQLite
    {
        private:
            // SQLite database
            sqlite3* db;

        public:
            /**
             * @brief How many seconds to wait for SQLite database file being available.
             * 
             */
            int wait_max_seconds;

            /**
             * @brief Construct a new mySQLite object.
             * 
             * @param db_file_nm Name of SQLite database file.
             * @param read_only true => SQLite database file is read-only. \n false => SQLite database file could be written into.
             * @param wait_max_seconds How many seconds to wait for SQLite database file being available.
             */
            mySQLite(const char* db_file_nm, const bool read_only, int wait_max_seconds);

            /**
             * @brief Destroy the mySQLite object.
             * 
             */
            ~mySQLite(){};

            /**
             * @brief Open connection to SQLite database file.
             * 
             * @param db_file_nm SQLite database file name.
             * @param read_only true => SQLite database file is read-only. \n false => SQLite database file could be written into.
             */
            void open(const char* db_file_nm, const bool read_only);

            /**
             * @brief Close connection to SQLite database file.
             * 
             */
            void close() const;

            /**
             * @brief Vacuum SQLite database file.
             * 
             */
            void vacuum() const;

            /**
             * @brief Execute SQL query, e.g. DELETE, DROP, INSERT commands.
             * 
             * @param sql SQL query to be executed.
             */
            void exec(const std::string& sql) const;

            /**
             * @brief Run SQL query, e.g. SELECT command.
             * 
             * @param sql SQL query to be run.
             * @return std::shared_ptr<lib_df::myDataFrame> Data retrieved via SQL query.
             */
            std::shared_ptr<lib_df::myDataFrame> query(const std::string& sql) const;

            /**
             * @brief Retrieve content of a table.
             * 
             * @param tbl_nm Name of the table to be retrieved.
             * @return std::shared_ptr<lib_df::myDataFrame> Data retrieved from the table.
             */
            std::shared_ptr<lib_df::myDataFrame> download_tbl(const std::string& tbl_nm);

            /**
             * @brief Upload table into SQLite database file.
             * 
             * @param tbl Table in form of myDataFrame object.
             * @param tbl_nm Name of the table to be created.
             * @param delete_old_data true => Delete existing data in the table before uploading the table. \n false => Add date to existing data when uploading the table.
             */
            void upload_tbl(const lib_df::myDataFrame& tbl, const std::string& tbl_nm, const bool delete_old_data);
    };
}
