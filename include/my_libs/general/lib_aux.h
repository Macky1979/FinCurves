/*
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

        std::string log_file_nm = "data/lib_aux/" + lib_aux::get_datestamp(1) + "_log.txt";
        bool clear_file = true;
        lib_aux::myLog log = lib_aux::myLog(log_file_nm, clear_file);
        log.write("This is a log message!");

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}
*/

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

namespace lib_aux
{
    // co-ordinates structure
    template <typename T>
    struct coordinates
    {
        T x;
        T y;
    };

    // implementation of tic()
    void tic();

    // implementation of toc()
    double toc();

    // implementation of timestamp
    std::string get_timestamp(const unsigned short& format_type = 0);

    // implementation of datestamp
    std::string get_datestamp(const unsigned short& format_type = 0);

    // split vector into several vectors of approximately same size => return indices which defines the new vectors
    std::vector<coordinates<int>> split_vector(const std::size_t& vector_length, const int& splits_no);

    // progress bar
    class myProgressBar
    {
        private:
            unsigned long cycles_no;
            unsigned long cycle;
            double progress;
            unsigned short progress_bar_len;
            std::string msg;
        public:
            // object functions
            unsigned long get_cycles_no() const {return this->cycles_no;};
            std::string get_msg() const {return this->msg;};
            void next();

            // object constructors
            myProgressBar(unsigned long cycles_no){this->cycles_no = cycles_no; this->cycle = 0; this->progress = 0; this->progress_bar_len = 40; this->msg = "";};

            // overloaded operator ++
            void operator++()
            {
                this->next();
            }
    };

    // logging object
    class myLog
    {
        private:
            std::string log_file_nm;
            std::ofstream log_file;
        public:
            // object functions
            std::string get_log_file_nm() const {return this->log_file_nm;};
            void write(std::string msg);

            // object constructors
            myLog(const std::string& log_file_nm, const bool& clear_file = true);

            // object destructor
            ~myLog();
    };

}