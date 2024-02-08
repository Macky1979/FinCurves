#include <iostream>
#include <string>
#include <memory>
#include "lib_math.h"
#include "lib_mtrx.h"

int lib_mtrx_examples()
{
    // create unite matrix
    {
        std::cout << "CREATE UNITE MATRIX" << std::endl;
        unsigned short rows_no = 3;
        unsigned short cols_no = 3;
        double element = 1.0;
        bool diagonal = true;
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = std::make_shared<lib_mtrx::myMatrix2D>(rows_no, cols_no, element, diagonal);
        mtrx->print();
        std::cout << '\n' << std::endl;
    }

    // create matrix of zeros
    {
        std::cout << "CREATE MATRIX OF ZEROS" << std::endl;
        unsigned short rows_no = 2;
        unsigned short cols_no = 5;
        double element = 0.0;
        bool diagonal = false;
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(rows_no, cols_no, element, diagonal);
        mtrx.print();
        std::cout << '\n' << std::endl;
    }

    // create matrix from a vector of doubles
    {
        std::cout << "CREATE MATRIX FROM ARRAY OF DOUBLES" << std::endl;
        std::vector<std::vector<double>> elements = 
            {{0.9037531, 0.0812568, 0.0080906, 0.0018606, 0.0040098, 0.0003044, 0.0001500, 0.0005748},
             {0.1213350, 0.7799222, 0.0617936, 0.0178147, 0.0134249, 0.0018724, 0.0010083, 0.0028288},
             {0.0397476, 0.3592938, 0.4364767, 0.0973718, 0.0457544, 0.0076005, 0.0031961, 0.0105591},
             {0.0160208, 0.2135963, 0.2467575, 0.2999252, 0.1347754, 0.0317650, 0.0139655, 0.0431944},
             {0.0404306, 0.1693487, 0.1340239, 0.1920848, 0.2618114, 0.0733376, 0.0290803, 0.0998829},
             {0.0065295, 0.0499845, 0.0600477, 0.1525826, 0.2277470, 0.1462818, 0.0610217, 0.2958053},
             {0.0030775, 0.0383176, 0.0394161, 0.1121800, 0.1444259, 0.0986995, 0.0680849, 0.4957984},
             {0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 1.0000000}};

        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = std::make_shared<lib_mtrx::myMatrix2D>(elements);
        mtrx->print();
        std::cout << '\n' << std::endl;
    }

    // load matrix from a .csv file
    {
        std::cout << "LOAD MATRIX FROM A .CSV FILE" << std::endl;
        std::string path = "data/lib_mtrx/";
        std::string mtrx_file_nm = "mtrx_A.csv";
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(path + mtrx_file_nm);
        mtrx.print();
        std::cout << '\n' << std::endl;
    }
 
    // copy constructor
    {
        std::cout << "COPY CONSTRUCTOR" << std::endl;
        
        // create matrix
        unsigned short rows_no = 2;
        unsigned short cols_no = 3;
        std::vector<std::vector<double>> elements =
            {{0.9037531, 0.0812568, 0.0080906},
             {0.0018606, 0.0040098, 0.0003044}};
        lib_mtrx::myMatrix2D mtrx_1 = lib_mtrx::myMatrix2D(elements);

        // copy constructor
        std::cout << "Pointer Copy Constructor" << std::endl;
        lib_mtrx::myMatrix2D mtrx_2 = lib_mtrx::myMatrix2D(mtrx_1);
        std::cout << "original matrix" << std::endl;
        mtrx_1.print();
        std::cout << "copied matrix" << std::endl;
        mtrx_2.print();

        // pointer copy constructor
        std::cout << "Reference Copy Constructor" << std::endl;
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_3 = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_1);
        std::cout << "original matrix" << std::endl;
        mtrx_1.print();
        std::cout << "copied matrix" << std::endl;
        mtrx_3->print();
    
        std::cout << '\n' << std::endl;
    }

    // transpose matrix
    {
        std::cout << "TRANSPOSE MATRIX" << std::endl;
        std::vector<std::vector<double>> elements = 
            {{0.9037531, 0.0812568, 0.0080906},
             {0.0018606, 0.0040098, 0.0003044}};
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(elements);
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_t = mtrx.transpose();
        std::cout << "original matrix" << std::endl;
        mtrx.print();
        std::cout << "transposed matrix" << std::endl;
        mtrx_t->print();
        std::cout << '\n' << std::endl;
    } 

    // sum range
    {
        std::cout << "SUM RANGE: 1 + 2 + 4 + 5 = 12" << std::endl;
        
        // create matrix
        unsigned short rows_no = 2;
        unsigned short cols_no = 3;
        std::vector<std::vector<double>> elements =
            {{1, 2, 3},
             {4, 5, 6}};
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(elements);
        mtrx.print();

        // 1 + 2 + 4 + 5 = 12
        double sum = 0.0;
        sum = mtrx.sum("0:2,0:2");
        std::cout << "mtrx.sum(0:2,0:2) = " << std::to_string(sum) << std::endl;
        sum = mtrx.sum(0, 2, 0, 2);
        std::cout << "mtrx.sum(0,2,0,2) = " << std::to_string(sum) << std::endl;

        std::cout << '\n' << std::endl;
    }

    // get row
    {
        std::cout << "GET ROW" << std::endl;
        
        // create matrix
        std::vector<std::vector<double>> mtrx_elements =
            {{1, 2},
             {3, 4}};
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(mtrx_elements);
        std::cout << "original matrix" << std::endl;
        mtrx.print();

        // get the second row
        std::shared_ptr<lib_mtrx::myMatrix2D> row = std::make_shared<lib_mtrx::myMatrix2D>(mtrx.get_row(1));
        std::cout << "second row" << std::endl;
        row->print();

        std::cout << '\n' << std::endl;
    }    

    // get column
    {
        std::cout << "GET COLUMN" << std::endl;
        
        // create matrix
        std::vector<std::vector<double>> mtrx_elements =
            {{1, 2},
             {3, 4}};
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(mtrx_elements);
        std::cout << "original matrix" << std::endl;
        mtrx.print();

        // get the first column
        std::shared_ptr<lib_mtrx::myMatrix2D> col = std::make_shared<lib_mtrx::myMatrix2D>(mtrx.get_col(0));
        std::cout << "first column" << std::endl;
        col->print();

        std::cout << '\n' << std::endl;
    } 

    // replace column
    {
        std::cout << "REPLACE COLUMN" << std::endl;
        
        // create matrix
        std::vector<std::vector<double>> mtrx_elements =
            {{1, 2},
             {3, 4}};
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(mtrx_elements);
        std::cout << "original matrix" << std::endl;
        mtrx.print();

        // create column
        std::vector<std::vector<double>> vector_elements =
            {{1},
             {3}};
        std::shared_ptr<lib_mtrx::myMatrix2D> vector = std::make_shared<lib_mtrx::myMatrix2D>(vector_elements);
        std::cout << "replacement column" << std::endl;
        vector->print();

        // replace column
        mtrx.replace_col(vector, 1);
        std::cout << "matrix after replacement" << std::endl;
        mtrx.print();

        std::cout << '\n' << std::endl;
    }

    // replace row
    {
        std::cout << "REPLACE ROW" << std::endl;
        
        // create matrix
        std::vector<std::vector<double>> mtrx_elements =
            {{1, 2},
             {3, 4}};
        lib_mtrx::myMatrix2D mtrx = lib_mtrx::myMatrix2D(mtrx_elements);
        std::cout << "original matrix" << std::endl;
        mtrx.print();

        // create column
        std::vector<std::vector<double>> vector_elements = {{1, 2}};
        std::shared_ptr<lib_mtrx::myMatrix2D> vector = std::make_shared<lib_mtrx::myMatrix2D>(vector_elements);
        std::cout << "replacement row" << std::endl;
        vector->print();

        // replace column
        mtrx.replace_row(vector, 1);
        std::cout << "matrix after replacement" << std::endl;
        mtrx.print();

        std::cout << '\n' << std::endl;
    }

    // add scalar
    {
        std::cout << "ADD SCALAR OF 2.0" << std::endl;
        
        // create matrix
        std::vector<std::vector<double>> mtrx_elements =
            {{1, 2},
             {3, 4}};
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_elements);
        std::cout << "original matrix" << std::endl;
        mtrx->print();

        // add scalar
        mtrx = lib_mtrx::add_scalar(mtrx, 2.0);
        std::cout << "matrix after adding 2.0" << std::endl;
        mtrx->print();

        std::cout << '\n' << std::endl;
    }

    // multiply with scalar
    {
        std::cout << "MULTIPLY WITH SCALAR OF 2.0" << std::endl;
        
        // create matrix
        std::vector<std::vector<double>> mtrx_elements =
            {{1, 2},
             {3, 4}};
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_elements);
        std::cout << "original matrix" << std::endl;
        mtrx->print();

        // multiply with scalar
        mtrx = lib_mtrx::mult_scalar(mtrx, 2.0);
        std::cout << "matrix after multiplying with 2.0" << std::endl;
        mtrx->print();

        std::cout << '\n' << std::endl;
    }

    // sum matrices
    {
        std::cout << "SUM MATRICES" << std::endl;
        
        // create matrices
        std::vector<std::vector<double>> mtrx_elements_1 =
            {{1, 2},
             {3, 4}};
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_1 = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_elements_1);
        std::cout << "matrix 1" << std::endl;
        mtrx_1->print();

        std::vector<std::vector<double>> mtrx_elements_2 =
            {{1, 1},
             {2, 2}};
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_2 = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_elements_2);
        std::cout << "matrix 2" << std::endl;
        mtrx_2->print();

        // sum the two matrices
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = lib_mtrx::add_mtrx(std::vector<std::shared_ptr<lib_mtrx::myMatrix2D>>{mtrx_1, mtrx_2});
        std::cout << "sum of the two matrices" << std::endl;
        mtrx->print();

        std::cout << '\n' << std::endl;
    }

    // multiply matrices
    {
        std::cout << "MULTIPLY MATRICES" << std::endl;
        
        // create matrices
        std::vector<std::vector<double>> mtrx_elements_1 = 
            {{1, 2},
             {3, 4},
             {5, 6}};
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_1 = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_elements_1);
        std::cout << "matrix 1" << std::endl;
        mtrx_1->print();

        std::vector<std::vector<double>> mtrx_elements_2 =
            {{1, 2, 3},
             {4, 5, 6}};
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx_2 = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_elements_2);
        std::cout << "matrix 2" << std::endl;
        mtrx_2->print();

        // multiplication of the two matrices
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = lib_mtrx::mult_mtrx(std::vector<std::shared_ptr<lib_mtrx::myMatrix2D>>{mtrx_1, mtrx_2});
        std::cout << "multiplication of the two matrices" << std::endl;
        mtrx->print();

        std::cout << '\n' << std::endl;
    }

    // store matrix into a .csv file
    {
        std::cout << "STORE MATRIX INTO .CSV FILE" << std::endl;
        
        // create matrices
        std::vector<std::vector<double>> mtrx_elements =
            {{1, 2},
             {3, 4},
             {5, 6}};
        std::shared_ptr<lib_mtrx::myMatrix2D> mtrx = std::make_shared<lib_mtrx::myMatrix2D>(mtrx_elements);
        mtrx->print();
        mtrx->save("data/lib_mtrx/saved_mtrx.csv");
        std::cout << '\n' << std::endl;
    }

    return 0;
}