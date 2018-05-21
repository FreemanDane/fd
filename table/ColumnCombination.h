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
    int operator[] (unsigned) const;
    void add(unsigned index);
    void remove(unsigned index);
    unsigned size() const;
    unsigned maxsize() const;
    unsigned getCombination() const;
    friend ColumnCombination operator+(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator*(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator!(const ColumnCombination &) ;
    ColumnCombination intersection(const ColumnCombination &) const;
    ColumnCombination convergence(const ColumnCombination &) const;
    ColumnCombination complement() const;
    category getCategory() const;
    void setCategory(category c);
};

#endif //FD_COLUMNCOMBINATION_H
