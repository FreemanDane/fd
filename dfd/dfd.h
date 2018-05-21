//
// Created by fengs on 2018/5/20.
//

#ifndef FD_DFD_H
#define FD_DFD_H

#include <vector>
#include "../table/ColumnCombination.h"
using std::vector;

FunctionalDependency* dfdmain(const string & filename);
vector<ColumnCombination> findLHSs(ColumnCombination RHS, ColumnCombination total);

#endif //FD_DFD_H
