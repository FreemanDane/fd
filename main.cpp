#include "tane/TANE.h"
#include <iostream>
#include <fstream>

int main() {
    Table table = readData("test/data.txt");
    TANE tane(table);
    tane.compute();
    std::ofstream out("test/result.txt");
    tane.output_result(out);
    return 0;
}

