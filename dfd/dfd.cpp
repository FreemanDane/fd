//
// Created by fengs on 2018/5/20.
//

#include "../table/Table.h"
#include "../table/ColumnCombination.h"
#include "../table/Partition.h"
#include "../table/FunctionalDependency.h"
#include "dfd.h"
#include <string>
#include <fstream>
using std::string;
using std::ofstream;

FunctionalDependency* dfdmain(const string & filename) {
    Table tbl = readData(filename);
    int num_col = tbl[0].size();
    int num_com = 1 << num_col;
    vector<FunctionalDependency> result;
    ColumnCombination total(num_com - 1, &tbl);
    Partition *parts = new Partition[num_com];
    for (int i = 0; i < num_com; ++i) {
        parts[i].size = 0;
    }
    for (int i = 0; i < num_col; ++i) {
        int num = 1 << i;
        parts[num] = getPartition(num, tbl);
    }
    for (int i = 0; i < num_col; ++i) {
        int com1 = 1 << i;
        bool unique = true;
        for (int j = 0; j < num_col; ++j) {
            int com2 = 1 << j;
            if (com1 == com2)
                continue;
            if (parts[com1 + com2].size == 0) {
                parts[com1 + com2] = getPartition(parts[com1], parts[com2], tbl);
            }
            if (parts[com1] == parts[com2] || parts[com1].size != parts[com1 + com2].size)
                unique = false;
        }
        if (unique) {
            total.remove(i);
            for (int j = 0; j < num_col; ++j) {
                if (i == j) {
                    continue;
                }
                result.push_back(FunctionalDependency(ColumnCombination(1 << i, &tbl), ColumnCombination(1 << j, &tbl)));
            }
        }
    }
    for (int i = 0; i < num_col; ++i) {
        if (total[i] == 0) {
            continue;
        }
        auto LHSs = findLHSs(ColumnCombination(1 << i, &tbl), total);
    }
}

vector<ColumnCombination> findLHSs(ColumnCombination RHS, ColumnCombination total) {
    ColumnCombination seeds(total);
    seeds = seeds * (!RHS);
    vector<ColumnCombination> result;
    while (seeds.size() != 0) {

    }
}