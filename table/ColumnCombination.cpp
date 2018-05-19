//
// Created by fengs on 2018/5/19.
//

#include "ColumnCombination.h"

ColumnCombination::ColumnCombination(Table *t) {
    combination = 0;
    tbl = t;
    total = (1U << t->size()) - 1;
    ctg = NONE;
}

ColumnCombination::ColumnCombination(unsigned cb, Table *t) {
    combination = cb;
    tbl = t;
    total = (1U << t->size()) - 1;
    ctg = NONE;
}

ColumnCombination::ColumnCombination(const ColumnCombination & cc) {
    combination = cc.combination;
    tbl = cc.tbl;
    total = cc.total;
    ctg = cc.ctg;
}

int ColumnCombination::operator[](unsigned index) {
    return (combination >> index) & 1U;
}

unsigned ColumnCombination::size() {
    unsigned result = 0, cc = combination;
    while (cc != 0){
        result += cc & 1u;
        cc = cc >> 1u;
    }

}

unsigned ColumnCombination::getCombination() {
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

ColumnCombination ColumnCombination::intersection(const ColumnCombination & c) {
    return ColumnCombination((combination & c.combination), tbl);
}

ColumnCombination ColumnCombination::convergence(const ColumnCombination & c) {
    return ColumnCombination((combination | c.combination), tbl);
}

ColumnCombination ColumnCombination::complement() {
    return ColumnCombination((~combination) & total, tbl);
}

category ColumnCombination::getCategory() {
    return ctg;
}

void ColumnCombination::setCategory(category c) {
    ctg = c;
}