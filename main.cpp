#include "dfd/dfd.h"
#include <iostream>
#include <algorithm>

using namespace std;
using std::sort;

int main(int argc, char const *args[]) {
    string filename = "data.txt";
    auto result = dfdmain(filename);
    string fileout = "out.txt";
    ofstream fout(fileout);
    sort(result.begin(),result.end());
    for (auto r : result) {
        fout << r << std::endl;
    }
    return 0;
    return 0;
}