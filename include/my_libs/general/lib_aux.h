/** \example lib_aux_examples.h
 * @file lib_aux.h
 * @author Michal Mackanic
 * @brief Various auxiliary C++ functions.
 * @version 1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

namespace lib_aux
{
    /**
     * @brief Template structure for [x, y] co-ordinate
     * 
     * @tparam T Template numerical type.
     */
    template <typename T>
    struct coordinates
    {
        T x;
        T y;
    };

    /**
     * @brief Implementation of tic() function. Functions tic() and toc() could be used to measure code runtime.
     * 
     */
    void tic();

    /**
     * @brief Implementation of toc() function. Functions tic() and toc() could be used to measure code runtime.
     * 
     * @return double Time in seconds elapsed between tic and toc.
     */
    double toc();

    /**
     * @brief Return timestamp.
     * 
     * @param format_type 0 => HH:MM:SS \n 1 => HHMMSS
     * @return std::string Timestamp
     */
    std::string get_timestamp(const unsigned short& format_type = 0);

    /**
     * @brief Return datestamp.
     * 
     * @param format_type 0 => DD/MM/YYYY HH:MM:SS \n 1 => YYYYMMDD_HHMMSS
     * @return std::string Datestamp
     */
    std::string get_datestamp(const unsigned short& format_type = 0);

    /**
     * @brief Split vector into several vectors of approximately same size. The function returns indices which define the new vectors. The functionality could be used to split dataset for purpose of multiprocessing.
     * 
     * @param vector_length Length of the original vector.
     * @param splits_no Number of the new vectors.
     * @return std::vector<coordinates<int>> Vector of indices defining new vectors.
     */
    std::vector<coordinates<int>> split_vector(const std::size_t& vector_length, const int& splits_no);

    /**
     * @brief Progress bar object.
     * 
     */
    class myProgressBar
    {
        private:
            unsigned long cycles_no;
            unsigned long cycle;
            double progress;
            unsigned short progress_bar_len;
            std::string msg;
        public:

            /**
             * @brief Get number of cycles to tracked by the progress bar.
             * 
             * @return unsigned long Number of cycles.
             */
            unsigned long get_cycles_no() const {return this->cycles_no;};

            /**
             * @brief Get progress bar string, i.e. [##### ].
             * 
             * @return std::string Progress bar string.
             */
            std::string get_msg() const {return this->msg;};

            /**
             * @brief Move to the next cycle.
             * 
             */
            void next();

            /**
             * @brief Construct a new myProgressBar object.
             * 
             * @param cycles_no Set number of cycles to be tracked by the progress bar.
             */
            myProgressBar(unsigned long cycles_no){this->cycles_no = cycles_no; this->cycle = 0; this->progress = 0; this->progress_bar_len = 40; this->msg = "";};

            /**
             * @brief Overload ++ operator with next() function.
             * 
             */
            void operator++()
            {
                this->next();
            }
    };

    /**
     * @brief Logging object that stores messages into a text file.
     * 
     */
    class myLog
    {
        private:
            std::string log_file_nm;
            std::ofstream log_file;
        public:
            /**
             * @brief Get the logging file name.
             * 
             * @return std::string Logging file name.
             */
            std::string get_log_file_nm() const {return this->log_file_nm;};
            void write(std::string msg);

            /**
             * @brief Construct a new myLog object.
             * 
             * @param log_file_nm Logging file name including path.
             * @param clear_file Clear logging file.
             */
            myLog(const std::string& log_file_nm, const bool& clear_file = true);

            /**
             * @brief Destroy the myLog object.
             * 
             */
            ~myLog();
    };

}