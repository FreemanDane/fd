//
// Created by fengs on 2018/5/19.
//

#include "ColumnCombination.h"

ColumnCombination::ColumnCombination(Table *t) {
    combination = 0;
    tbl = t;
    total = (1 << t->size()) - 1;
    ctg = None;
}

ColumnCombination::ColumnCombination(int cb, Table *t) {
    combination = cb;
    tbl = t;
    total = (1 << t->size()) - 1;
    ctg = NONE;
}

ColumnCombination::ColumnCombination(const ColumnCombination & cc) {
    combination = cc.combination;
    tbl = cc.tbl;
    total = cc.total;
    ctg = cc.ctg;
}

ColumnCombination::~ColumnCombination() {}

int ColumnCombination::operator[](int index) {
    return (combination >> index) & 1;
}

int ColumnCombination::getCombination() {
    return combination;
}

ColumnCombination operator+(const ColumnCombination & c1, const ColumnCombination & c2) {
    return ColumnCombination((c1.combination | c2.combination), c1.tbl);
}

ColumnCombination operator*(const ColumnCombination & c1, const ColumnCombination & c2){
    return ColumnCombination((c1.combination & c2.combination), c1.tbl);
}

ColumnCombination operator!(const ColumnCombination & c){
    return ColumnCombination(~c.combination & c.total, c.tbl);
}
