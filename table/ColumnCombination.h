//
// Created by fengs on 2018/5/19.
//

#ifndef FD_COLUMNCOMBINATION_H
#define FD_COLUMNCOMBINATION_H

#include "Table.h"

enum category
{
    NONE,
    DEPENDENCY,
    MINIMAL_DEPENDENCY,
    CANDIDATE_MINIMAL_DEPENDENCY,
    NON_DEPENDENCY,
    MAXIMAL_NON_DEPENDENCY,
    CANDIDATE_MAXIMAL_DENPENDENCY
};

class ColumnCombination
{
private:
    int combination;
    const int total;
    category ctg;
    Table *tbl;
public:
    ColumnCombination(Table *);
    ColumnCombination(int, Table *);
    ColumnCombination(const ColumnCombination &);
    ~ColumnCombination();
    int operator[] (int);
    int getCombination();
    friend ColumnCombination operator+(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator*(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator!(const ColumnCombination &);
    ColumnCombination intersection(const ColumnCombination &);
    ColumnCombination convergence(const ColumnCombination &);
    ColumnCombination complement();
    void computePartition(const ColumnCombination &, const ColumnCombination &);
    category getCategory();
    void setCategory(category c);
};

#endif //FD_COLUMNCOMBINATION_H
