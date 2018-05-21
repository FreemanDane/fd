//
// Created by fengs on 2018/5/19.
//

#include "ColumnCombination.h"

ColumnCombination::ColumnCombination(Table *t) {
    auto msize = (*tbl)[0].size();
    combination = 0;
    tbl = t;
    total = (1U << msize) - 1;
    ctg = NONE;
}

ColumnCombination::ColumnCombination(unsigned cb, Table *t) {
    auto msize = (*tbl)[0].size();
    combination = cb;
    tbl = t;
    total = (1U << msize) - 1;
    ctg = NONE;
}

ColumnCombination::ColumnCombination(const ColumnCombination & cc) {
    combination = cc.combination;
    tbl = cc.tbl;
    total = cc.total;
    ctg = cc.ctg;
}

int ColumnCombination::operator[](unsigned index) const {
    return (combination >> index) & 1U;
}

unsigned ColumnCombination::size() const {
    unsigned result = 0, cc = combination;
    while (cc != 0){
        result += cc & 1u;
        cc = cc >> 1u;
    }

}

unsigned ColumnCombination::maxsize() const {
    return (*tbl)[0].size();
}

unsigned ColumnCombination::getCombination() const {
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

ColumnCombination ColumnCombination::intersection(const ColumnCombination & c) const {
    return ColumnCombination((combination & c.combination), tbl);
}

ColumnCombination ColumnCombination::convergence(const ColumnCombination & c) const {
    return ColumnCombination((combination | c.combination), tbl);
}

ColumnCombination ColumnCombination::complement() const {
    return ColumnCombination((~combination) & total, tbl);
}

category ColumnCombination::getCategory() const {
    return ctg;
}

void ColumnCombination::setCategory(category c) {
    ctg = c;
}

void ColumnCombination::add(unsigned index) {
    combination |= (1u << index);
}

void ColumnCombination::remove(unsigned index) {
    combination &= ~(1u << index);
}