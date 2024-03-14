#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <stack>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "lib_str.h"
#include "lib_aux.h"

// global variable holding time elapsed between tic() and toc()
std::chrono::time_point<std::chrono::high_resolution_clock> tictoc_begin;

// implementation of tic()
void lib_aux::tic()
{
    tictoc_begin = std::chrono::high_resolution_clock::now();
}

// implementation of toc()
double lib_aux::toc()
{
    std::chrono::duration<double> elapsed_time (std::chrono::high_resolution_clock::now() - tictoc_begin);
    return elapsed_time.count();
}

// implementation of time stamp
std::string lib_aux::get_timestamp(const unsigned short& format_type)
{
    // define variables
    time_t rawtime;
    struct tm* timeinfo;
    char char_aux [20];
    std::string timeStr = "";

    // get time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // format time
    if (format_type == 0)
        strftime(char_aux, 20, "%H:%M:%S", timeinfo);
    else
        strftime(char_aux, 20, "%H%M%S", timeinfo);
    timeStr.assign(char_aux);

    // return time string
    return timeStr;
}

// implementation of date stamp
std::string lib_aux::get_datestamp(const unsigned short& format_type)
{
    // define variables
    time_t rawtime;
    struct tm* timeinfo;
    char char_aux [20];
    std::string timeStr = "";

    // get time
    time(&rawtime);
    timeinfo = localtime (&rawtime);

    // format time
    if (format_type == 0)
        strftime(char_aux, 20, "%d/%m/%Y %H:%M:%S", timeinfo);
    else
        strftime(char_aux, 20, "%Y%m%d_%H%M%S", timeinfo);
    timeStr.assign(char_aux);

    // return time string
    return timeStr;
}

// split vector into several vectors of approximately same size => return indices which defines the new vectors
std::vector<lib_aux::coordinates<int>> lib_aux::split_vector(const std::size_t& vector_length, const int& splits_no)
{
    // check that lenght of input vector is not lower than number of splits
    if (vector_length < splits_no)
    {
        throw std::invalid_argument((std::string)__func__ + ": Length of the input vector cannot be shorter than number of splits!");
    }

    // get number of elements per sub
    int elements_per_split = vector_length / splits_no;

    // create a vector holding position indicies of the newly created vectors
    std::vector<coordinates<int>> indicies;

    // determine position indicies of vectors created by the split of the original one
    int current_index = 0;
    for (int split_idx = 0; split_idx < splits_no; split_idx++)
    {
        // determine lower and upper position indicies of one particular split
        coordinates<int> index;
        index.x = current_index;
        current_index += elements_per_split;
        index.y = current_index - 1;

        // add the position indicies to the vector
        indicies.push_back(index);
    }

    // place the last remaining elements to the last vector
    indicies[splits_no - 1].y = vector_length - 1;

    // return vector with position indicies
    return indicies;
}

/*
 * myProgressBar
 */
void lib_aux::myProgressBar::next()
{
    // update cycle variable
    this->cycle++;
    if (this->cycle > this->cycles_no)
        this->cycle = this->cycles_no;

    // update progress variable
    this->progress = static_cast<double>(this->cycle) / this->cycles_no;

    // determine number of stars in message
    unsigned short hashes_no = std::round(this->progress * this->progress_bar_len);

    // construct message
    this->msg = "[";
    this->msg += lib_str::repeate_char("#", hashes_no);
    this->msg += lib_str::repeate_char(" ", this->progress_bar_len - hashes_no);
    this->msg += "] ";
    this->msg += lib_str::add_leading_char(std::to_string(static_cast<int>(this->progress * 100)), 4, ' ');
    this->msg += "%";

    // print progress bar
    std::cout << '\r' << std::flush;
    if (this->progress < 1.0)
    {
        std::cout << this->msg << std::flush;
    }
    else
        std::cout << this->msg << std::endl;
}

/*
 * myLog
 */

// object constructor
lib_aux::myLog::myLog(const std::string& log_file_nm, const bool& clear_file)
{
    // assign log file name
    this->log_file_nm = log_file_nm;

    // open log file
    if (clear_file)
        this->log_file.open(log_file_nm, std::ofstream::out | std::ofstream::trunc);
    else
        this->log_file.open(log_file_nm, std::ofstream::out | std::ofstream::app);

    // check that you were able to open the log file
    if(!this->log_file.is_open())
        throw std::runtime_error((std::string)__func__ + ": Unable to open log file " + log_file_nm + "!");
};

// object destructor
lib_aux::myLog::~myLog()
{
    this->log_file.close();
};

// object functions
void lib_aux::myLog::write(std::string msg)
{
    std::string timestamp = lib_aux::get_timestamp();
    std::cout << timestamp + " - " + msg << std::endl;
    this->log_file << timestamp + " - " + msg << '\n';
}