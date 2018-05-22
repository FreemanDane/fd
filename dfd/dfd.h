//
// Created by fengs on 2018/5/20.
//

#ifndef FD_DFD_H
#define FD_DFD_H

#include <vector>
#include <string>
#include "../table/Table.h"
#include "../table/ColumnCombination.h"
#include "../table/Partition.h"
#include "../table/FunctionalDependency.h"


enum Category {
    NOT_VISITED = 0x10,
    NONE = 0x20,
    CANDIDATE = 0x2,
    DEPENDENCY = 0x4,
    MAX_OR_MIN = 0x8
};

vector<FunctionalDependency> dfdmain(const std::string & filename);
vector<ColumnCombination> findLHSs(ColumnCombination RHS, ColumnCombination total, Partition *parts);
bool isMinimal(const ColumnCombination &, const ColumnCombination &, const ColumnCombination &, Category *ctg, int num_col);
bool isMaximal(const ColumnCombination &, const ColumnCombination &, const ColumnCombination &, Category *ctg, int num_col);
void inferCategory(const ColumnCombination & left, const ColumnCombination & right, const ColumnCombination & total, Category *ctg, int num_col);
void computePartition(const ColumnCombination & left, const ColumnCombination & right, Partition *parts, Category *ctg, int num_col);
void getPartition(const ColumnCombination & target, Partition *parts, int num_col);
ColumnCombination pickNextNode(const ColumnCombination & node, const ColumnCombination & RHS, const ColumnCombination & total, vector<ColumnCombination> seeds, Category *ctg, vector<ColumnCombination> &minDep, vector<ColumnCombination> &maxNonDep, int num_col);
std::vector<ColumnCombination> generateNextSeeds(vector<ColumnCombination> &minDep, vector<ColumnCombination> &maxNonDep, const ColumnCombination & target, const ColumnCombination & total, int num_col, Table *tbl);

#endif //FD_DFD_H
