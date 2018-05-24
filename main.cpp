#include <iostream>
#include <fstream>
#include <algorithm>
#include "tane/TANE.h"
#include "dfd/dfd.h"
#include "util.h"

bool end_with(const std::string& a, const std::string& b) {
    if (b.size() > a.size())
        return false;
    return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}


int main(int argc, char const *args[]) {
    if (argc == 1){
        std::cerr << "Missing algorithm operand\n";
        return 1;
    }
    if (argc == 2) {
        std::cerr << "Missing input file operand\n";
        return 1;
    }
    std::string algorithm = args[1], filename = args[2];
    if (!end_with(filename, ".txt")) {
        std::cerr << "Expecting a '.txt' file\n";
        return 1;
    }
    int thread_num;
    if (argc == 4)
        thread_num = std::stoi( args[3] );
    else
        thread_num = 1;
    std::string output_filename = filename.substr(0, filename.length() - 4) + "_result.txt";
    if(algorithm == "tane") {
        Table table = readData(filename);
        TANE tane(table, thread_num);
        if(thread_num > 1)
            tane.compute_multithreading();
        else
            tane.compute();
        std::ofstream out(output_filename);
        tane.output_result(out);
        return 0;
    } else if(algorithm == "dfd") {
        auto result = dfdmain(filename);
        ofstream fout(output_filename);
        std::sort(result.begin(),result.end());
        for (auto r : result) {
            fout << r << std::endl;
        }
        return 0;
    } else {
        std::cerr << algorithm << " is not implemented\n";
        return 1;
    }
}
