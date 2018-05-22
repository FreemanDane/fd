#include "TANE.h"
#include <algorithm>
#include <iostream>
#include <fstream>

int main() {
    int i = 5;
    std::vector<int>({i});
    Table table = readData("test/test_data.txt");
//    Table table = readData("test/data.txt");
    TANE tane(table);
    tane.compute();
    std::ofstream out("test/result.txt");
    tane.output_result(out);
    return 0;
}

