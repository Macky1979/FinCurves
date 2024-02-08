#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "lib_str.h"
#include "lib_tbl.h"

using namespace std;

void print_tbl(const std::vector<std::shared_ptr<std::string>>& tenors, const std::vector<std::shared_ptr<double>>& rates)
{
    std::string row;
    std::cout << lib_str::add_leading_char("=", 15, '=') << std::endl;

    row = lib_str::add_leading_char("tenor", 5, ' ');
    row += lib_str::add_leading_char("rate", 10, ' ');
    std::cout << row << std::endl;

    std::cout << lib_str::add_leading_char("=", 15, '=') << std::endl;

    for (std::size_t idx = 0; idx < tenors.size(); idx++)
    {
        if (tenors[idx] == nullptr)
            row = lib_str::add_leading_char("NULL", 5, ' ');
        else
            row = lib_str::add_leading_char(*tenors[idx], 5, ' ');
        
        if (rates[idx] == nullptr)
            row += lib_str::add_leading_char("NULL", 10, ' ');
        else
            row += lib_str::add_leading_char(std::to_string(*rates[idx]), 10, ' ');

        std::cout << row << std::endl;
    }

    std::cout << lib_str::add_leading_char("=", 15, '=') << std::endl;
}

int main()
{
    std::string name = "";

    // variables
    string input_file_nm = "/home/macky/Documents/Programming/C++/FinCurves/include/my_libs/general/data/lib_tbl/eur_ri_ester_20220429.csv";
    string output_file_nm = "include/my_libs/general/data/lib_tbl/ouput.csv";
    std::shared_ptr<lib_tbl::myTable> tbl1 = std::make_shared<lib_tbl::myTable>();
    std::shared_ptr<lib_tbl::myTable> tbl2 = std::make_shared<lib_tbl::myTable>();
    string sep = ",";
    bool quotes = false;
    std::size_t rows_no = 10;

    // load table from a file
    tbl1->read(input_file_nm, sep, quotes);
    std::cout << "tbl1 - all columns" << std::endl;
    tbl1->print(rows_no);
    std::cout << '\n' << std::endl;

    // copy table
    tbl2->copy(tbl1);

    // clear the first table
    tbl1->clear();

    // print the second table
    std::cout << "tbl2 - all columns" << std::endl;
    tbl2->print(rows_no);
    std::cout << '\n' << std::endl;

    // extract columns - all rows
    std::vector<std::shared_ptr<std::string>> tenors1 = tbl2->get_str_column("tenor");
    std::vector<std::shared_ptr<double>> rates1 = tbl2->get_double_column("rate");

    std::cout << "tbl2 extract - selected columns; all rows" << std::endl;
    print_tbl(tenors1, rates1);
    std::cout << '\n' << std::endl;

    // extract columns - exclude N/A values
    std::vector<std::size_t> rows_idx = tbl2->get_rows_idx("rate", 1);
    std::vector<std::shared_ptr<std::string>> tenors2 = tbl2->get_str_column("tenor", rows_idx);
    std::vector<std::shared_ptr<double>> rates2 = tbl2->get_double_column("rate", rows_idx);

    std::cout << "tbl2 - selected columns; drop rows with N/A value in 'rate' column" << std::endl;
    print_tbl(tenors2, rates2);
    std::cout << '\n' << std::endl;

    // save the second table
    tbl2->write(output_file_nm, sep, quotes);

    // everything OK
    return 0;
}