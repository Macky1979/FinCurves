/*
#include <string>
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include "lib_df.h"
#include "lib_sql.h"
#include "lib_aux.h"

void print_df(std::shared_ptr<lib_df::myDataFrame> df)
{
    // print result of SQL query
    for (int i = 0; i < df->tbl.values.size(); i++)
    {
        for (int j = 0; j < df->tbl.values[0].size(); j++)
        {
            std::cout << df->tbl.values[i][j] << " ";
        }
        std::cout << '\n';
    }
}

int main()
{
    // variables
    const char * db_file_nm = "data/lib_sql/cities.db";
    std::string sql_file_nm = "data/lib_sql/cities.sql";
    std::string sql;
    std::shared_ptr<lib_df::myDataFrame> rslt = std::make_shared<lib_df::myDataFrame>();
    bool read_only;
    int wait_max_seconds = 10;
    bool delete_old_data = false;

    // create SQLite object and open connection to SQLite database file in read-write mode
    read_only = false;
    lib_sql::mySQLite db(db_file_nm, read_only, wait_max_seconds);


    // create table if it does not exists
    sql = lib_sql::read_sql(sql_file_nm, "create_tbl");
    db.exec(sql);

    // delete table
    sql = lib_sql::read_sql(sql_file_nm, "delete_tbl");
    db.exec("DELETE FROM cities;");   

    // insert data into table
    sql = lib_sql::read_sql(sql_file_nm, "insert_into_tbl");
    db.exec("INSERT INTO cities (city, country) VALUES ('Prague', 'Czech Republic');");
    
    // vacuum SQLite database file to avoid its excessive growth
    db.vacuum();

    // close connection to SQLite database file
    db.close();

    // open connection to SQLite database file in read-only mode
    read_only = true;
    db.open(db_file_nm, read_only);

    // query database
    sql = lib_sql::read_sql(sql_file_nm, "select_from_tbl");
    rslt = db.query(sql);

    // print dataframe
    print_df(rslt);

    rslt = db.download_tbl("cities");

    // print dataframe
    print_df(rslt);

    // close connection to SQLite database file
    db.close();

    // open connection to SQLite database file in read-write mode
    read_only = false;
    db.open(db_file_nm, read_only);

    // re-insert dataframe into the table
    db.upload_tbl(*rslt, "cities", delete_old_data);

    // print dataframe
    sql = lib_sql::read_sql(sql_file_nm, "select_from_tbl");
    rslt = db.query(sql);

    // print dataframe
    print_df(rslt);

    // close connection to SQLite database file
    db.close();

    // demonstrate function replace_in_sql()
    sql = "SELECT ##col_nm## FROM cities;";
    std::string replace_what = "##col_nm##";
    std::string replace_with = "cities";
    sql = lib_sql::replace_in_sql(sql, replace_what, replace_with);
    std::cout << sql << std::endl;

    // everything OK
    return 0;
}
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
    // read SQL query from a text file
    std::string read_sql(const std::string& sql_file_nm, const std::string& tag);

    // make substitutions in SQL query
    std::string replace_in_sql(std::string sql, std::string replace_what, std::string replace_with);

    // define object that handles SQLite database
    class mySQLite
    {
        private:
            // SQLite database
            sqlite3* db;

        public:
            // how many seconds to wait for SQLite database file being available
            int wait_max_seconds;

            // object constructors
            mySQLite(const char* db_file_nm, const bool read_only, int wait_max_seconds);

            // object destructor
            ~mySQLite(){};

            // object function declarations
            void open(const char* db_file_nm, const bool read_only);
            void close() const;
            void vacuum() const;
            void exec(const std::string& sql) const;
            std::shared_ptr<lib_df::myDataFrame> query(const std::string& sql) const;
            std::shared_ptr<lib_df::myDataFrame> download_tbl(const std::string& tbl_nm);
            void upload_tbl(const lib_df::myDataFrame& tbl, const std::string& tbl_nm, const bool delete_old_data);
    };
}
