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
    unsigned combination;
    unsigned total;
    category ctg;
    Table *tbl;
public:
    explicit ColumnCombination(Table *);
    ColumnCombination(unsigned, Table *);
    ColumnCombination(const ColumnCombination &);
    ~ColumnCombination() = default;
    int operator[] (unsigned);
    unsigned size();
    unsigned getCombination();
    friend ColumnCombination operator+(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator*(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator!(const ColumnCombination &);
    ColumnCombination intersection(const ColumnCombination &);
    ColumnCombination convergence(const ColumnCombination &);
    ColumnCombination complement();
    category getCategory();
    void setCategory(category c);
};

#endif //FD_COLUMNCOMBINATION_H
