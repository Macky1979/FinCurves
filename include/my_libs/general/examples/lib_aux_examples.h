#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "lib_aux.h"

int main()
{
    // tic & toc
    {
        std::cout << "TIC & TOC EXAMPLE" << std::endl;
        lib_aux::tic();
        std::cout << "We are waiting to demonstrate tic() & toc() functions..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "We waited for approximately " + std::to_string(lib_aux::toc()) + " seconds!" << std::endl;
        std::cout << '\n' << std::endl;
    }

    // time and date stamp
    {
        std::cout << "TIME AND DATE STAMP" << std::endl;
        std::cout << "time stamp: " + lib_aux::get_timestamp() << std::endl;
        std::cout << "date stamp: " + lib_aux::get_datestamp() << std::endl;
        std::cout << '\n' << std::endl;
    }

    // vector split - used in multiprocessing
    {
        std::cout << "VECTOR SPLIT - USED IN MULTIPROCESSING" << std::endl;
        int vector_length = 10;
        int splits_no = 4;
        std::vector<lib_aux::coordinates<int>> indicies = lib_aux::split_vector(vector_length, splits_no);
        for (unsigned short idx = 0; idx < splits_no; idx++)
        {
            std::cout << "batch: " + std::to_string(idx) << std::endl;
            std::cout << "   begin at " << std::to_string(indicies[idx].x) << " position" << std::endl;
            std::cout << "   end at " << std::to_string(indicies[idx].y) << " position" << std::endl;
        }
        std::cout << '\n' << std::endl;
    }

    // progress bar
    {
        std::cout << "PROGRESS BAR" << std::endl;
        lib_aux::myProgressBar bar = lib_aux::myProgressBar(15);
        for (unsigned i = 0; i < bar.get_cycles_no(); i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // pretend some work
            ++bar;
        }
        std::cout << '\n' << std::endl;
    }

    // logging
    {
        std::cout << "LOGGING" << std::endl;

        std::string log_file_nm = "/home/macky/Documents/Programming/C++/FinCurves/include/my_libs/general/data/lib_aux/" + lib_aux::get_datestamp(1) + "_log.txt";
        bool clear_file = true;
        lib_aux::myLog log = lib_aux::myLog(log_file_nm, clear_file);
        log.write("This is a log message!");

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}