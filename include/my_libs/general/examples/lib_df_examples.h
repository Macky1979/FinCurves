#include <string>
#include <iostream>
#include <vector>
#include "lib_sql.h"
#include "lib_df.h"

using namespace std;

int main()
{

    std::string name = "";

    // variables
    const char * db_file_nm = "include/my_libs/general/data/lib_sql/database.db";
    string file_nm = "include/my_libs/general/data/lib_sql/cities.csv";
    std::shared_ptr<lib_df::myDataFrame> rslt1 = std::make_shared<lib_df::myDataFrame>();
    std::shared_ptr<lib_df::myDataFrame> rslt2 = std::make_shared<lib_df::myDataFrame>();
    bool read_only = false;
    int wait_max_seconds = 10;
    bool delete_old_data = true;
    string sep = ";";
    bool quotes = true;

    // create SQLite object and open connection to SQLite database file in read-write mode
    lib_sql::mySQLite db(db_file_nm, read_only, wait_max_seconds);

    // create table if it does not exists
    db.exec("CREATE TABLE IF NOT EXISTS cities (city VARCHAR(20), country VARCHAR(20));");

    // delete table
    db.exec("DELETE FROM cities;");

    // insert data into table
    db.exec("INSERT INTO cities (city, country) VALUES ('Prague', 'Czech Republic');");
    
    // vacuum SQLite database file to avoid its excessive growth
    db.vacuum();

    // download table
    rslt1 = db.download_tbl("cities");

    // close connection to SQLite database file
    db.close();

    // write dataframe into a .csv file
    rslt1->write(file_nm, sep, quotes);

    // create dataframe from a .csv file
    rslt2->read(file_nm, sep, quotes);
    rslt2->print();

    // everything OK
    return 0;
}