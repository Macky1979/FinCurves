#include <iostream>
#include <fstream>
#include <string>
#include "lib_file.h"

int main()
{
    // create a folder ~/test_cpp
    {
        std::cout << "CREATE A FOLDER /home/macky/test_cpp" << std::endl;
        std::string path_nm = "/home/macky/test_cpp";
        if (lib_file::create_folder(path_nm))
            std::cout << "Folder " + path_nm + " created!" << std::endl;
        else
            std::cout << "Could not create folder " + path_nm + "!" << std::endl;
        std::cout << '\n' << std::endl;
    }

    // check existence of a folder ~/test_cpp
    {
        std::cout << "CHECK EXISTANCE OF /home/macky/test_cpp" << std::endl;
        std::string path_nm = "/home/macky/test_cpp";
        if (lib_file::path_exists(path_nm))
            std::cout << "Folder " + path_nm + " exists" << std::endl;
        else
            std::cout << "Folder " + path_nm + " does not exist" << std::endl;
        std::cout << '\n' << std::endl;
    }
 
    // get list of files in folder
    {
        std::cout << "GET LIST OF FILES IN FOLDER /home/macky/test_cpp" << std::endl;
        std::string path_nm = "/home/macky/test_cpp";
        
        std::ofstream f1(path_nm + "/file_1.txt");
        f1 << "This is the first file!\n"; 
        f1.close();

        std::ofstream f2(path_nm + "/file_2.txt");
        f2 << "This is the second file!\n"; 
        f2.close();

        std::ofstream f3(path_nm + "/file_3.txt");
        f3 << "This is the third file!\n"; 
        f3.close();

        std::vector<std::string> file_nms = lib_file::get_file_list(path_nm);
        for (unsigned short idx = 0; idx < file_nms.size(); idx++)
            std::cout << file_nms[idx] << std::endl;

        std::cout << '\n' << std::endl;
    }

    // delete a single file
    {
        std::cout << "DELETE A SINGLE FILE IN FOLDER /home/macky/test_cpp" << std::endl;
        std::string path_nm = "/home/macky/test_cpp/file_1.txt";
        if (lib_file::delete_path (path_nm))
            std::cout << "File " + path_nm + " deleted!" << std::endl;
        else
            std::cout << "Could not delete file " + path_nm + "!" << std::endl;
        
        std::cout << '\n' << std::endl;
    }

    // delete files stored in a vector
    {
        std::cout << "DELETE FILES IN FOLDER /home/macky/test_cpp" << std::endl;
        std::string path_nm = "/home/macky/test_cpp/file_2.txt";
        std::vector<std::string> path_nms = {path_nm};
        unsigned short deleted_files_no = lib_file::delete_paths(path_nms);
        std::cout << std::to_string(deleted_files_no) + " files deleted!" << std::endl;

        std::cout << '\n' << std::endl;  
    }

    // delete all files in folder
    {
        std::cout << "DELETE ALL FILES IN FOLDER /home/macky/test_cpp" << std::endl;
        std::string path_nm = "/home/macky/test_cpp";
        unsigned short deleted_files_no = lib_file::delete_all_files(path_nm);
        std::cout << std::to_string(deleted_files_no) + " files deleted!" << std::endl;
        
        std::cout << '\n' << std::endl;
    }

    // delete folder
    {
        std::cout << "DELETE FOLDER /home/macky/test_cpp" << std::endl;
        std::string path_nm = "/home/macky/test_cpp";
        if (lib_file::create_folder(path_nm))
            std::cout << "Folder " + path_nm + " deleted!";
        else
            std::cout << "Could not delete folder " + path_nm + "!";

        std::cout << '\n' << std::endl;
    }

    // everything OK
    return 0;
}