//
// Created by fengs on 2018/5/19.
//

#ifndef FD_FUNCTIONALDEPENDENCY_H
#define FD_FUNCTIONALDEPENDENCY_H

#include "ColumnCombination.h"
#include <fstream>

using std::fstream;
using std::ofstream;

class FunctionalDependency
{
private:
    ColumnCombination left;
    ColumnCombination right;
public:
    FunctionalDependency(const ColumnCombination &, const ColumnCombination &);
    ~FunctionalDependency() = default;
    friend ofstream & operator << (ofstream &, FunctionalDependency);
    friend bool operator < (const FunctionalDependency &, const FunctionalDependency &);
    friend bool operator == (const FunctionalDependency &, const FunctionalDependency &);
    friend bool operator > (const FunctionalDependency &, const FunctionalDependency &);
    friend bool operator >= (const FunctionalDependency &, const FunctionalDependency &);
    friend bool operator <= (const FunctionalDependency &, const FunctionalDependency &);
};

#endif //FD_FUNCTIONALDEPENDENCY_H
