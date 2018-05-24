//
// Created by chehang on 2018/5/23.
//

#include "AttrIndexes.h"

void AttrIndexes::insert_into(int new_index) {
    indexes |= 1 << new_index;
}

AttrIndexes AttrIndexes::merge(const AttrIndexes& other) const {
    AttrIndexes ret(indexes | other.indexes);
    return ret;
}

AttrIndexes AttrIndexes::difference(const AttrIndexes &other) const {
    AttrIndexes ret(indexes ^ (indexes & other.indexes));
    return ret;
}

AttrIndexes AttrIndexes::intersection(const AttrIndexes &other) const {
    AttrIndexes ret(indexes & other.indexes);
    return ret;
}

int AttrIndexes::size() const {
    int l = 0;
    for(int i = 0; i < 8 * sizeof(int); ++i){
        if(indexes & (1 << i))
            ++l;
    }
    return l;
}

bool AttrIndexes::operator<(const AttrIndexes &a) const {
    auto i = begin();
    auto j = a.begin();
    for (; i != end() && j != a.end(); ++i, ++j){
        if(*i < *j)
            return true;
        else if(*i > *j)
            return false;
    }
    if (j != a.end())
        return true;
    return false;
}

bool AttrIndexes::operator==(const AttrIndexes &a) const {
    return indexes == a.indexes;
}

std::size_t AttrIndexesHasher::operator()(AttrIndexes const& a) const {
    return std::hash<int>()(a.index());
}


