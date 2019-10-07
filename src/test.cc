#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "csv.h"


int main (int argc, char *argv[])
{
    std::string ifilename;
    std::string ofilename;

    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " /path/to/input.csv [/path/to/output.csv]" << std::endl;
        return 1;
    }
    else if (argc <= 2) {
        ifilename = argv[1];
        ofilename = argv[1];
    }
    else {
        ifilename = argv[1];
        ofilename = argv[2];
    }

    //std::cout << ifilename << " >> " << ofilename << std::endl;

    CSV my_csv;
    my_csv.sep = ' ';
    my_csv.col_delimiter = " ] ";
    my_csv.read(ifilename, true);
    std::cout << my_csv;

    my_csv.sep = ',';
    my_csv.append(my_csv);
    my_csv.write(std::cout);

    return 0;
}
