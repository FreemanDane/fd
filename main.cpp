#include "dfd/dfd.h"
#include <iostream>

using namespace std;

int main(int argc, char const *args[]) {
    string filename = "test_data.txt";
    auto result = dfdmain(filename);
    string fileout = "out.txt";
    ofstream fout(fileout);
    for (auto r : result) {
        fout << r << std::endl;
    }
    return 0;
    return 0;
}