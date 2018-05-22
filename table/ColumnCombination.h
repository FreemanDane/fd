//
// Created by fengs on 2018/5/19.
//

#ifndef FD_COLUMNCOMBINATION_H
#define FD_COLUMNCOMBINATION_H

#include "Table.h"

class ColumnCombination
{
private:
    unsigned combination;
    unsigned total;
    int msize;
    Table *tbl;
    bool isNull;
public:
    explicit ColumnCombination(Table *);
    ColumnCombination(unsigned, Table *);
    ColumnCombination(const ColumnCombination &);
    ~ColumnCombination() = default;
    int operator[] (unsigned) const;
    void add(unsigned index);
    void remove(unsigned index);
    Table *getTable() const;
    bool isSubset(const ColumnCombination &) const;
    bool isSuperset(const ColumnCombination &) const;
    int size() const;
    unsigned maxsize() const;
    unsigned getCombination() const;
    friend ColumnCombination operator/(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator+(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator*(const ColumnCombination &, const ColumnCombination &);
    friend ColumnCombination operator!(const ColumnCombination &) ;
    friend bool operator==(const ColumnCombination &, const ColumnCombination &);
    ColumnCombination intersection(const ColumnCombination &) const;
    ColumnCombination convergence(const ColumnCombination &) const;
    ColumnCombination complement() const;
};

#endif //FD_COLUMNCOMBINATION_H
