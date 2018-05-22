//
// Created by chehang on 2018/5/18.
//

#include "TANE.h"
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "../util.h"


void AttrIndexes::insert_into(int new_index) {
    auto it = std::lower_bound( begin(), end(), new_index );
    insert(it, new_index);
}

AttrIndexes AttrIndexes::merge(const AttrIndexes& other) const {
    AttrIndexes ret;
    std::set_union(begin(), end(), other.begin(), other.end(),
                   std::inserter(ret, ret.begin()));
    return ret;
}

AttrIndexes AttrIndexes::difference(const AttrIndexes &other) const {
    AttrIndexes ret;
    std::set_difference(begin(), end(), other.begin(), other.end(),
                        std::inserter(ret, ret.begin()));
    return ret;
}

AttrIndexes AttrIndexes::intersection(const AttrIndexes &other) const {
    AttrIndexes ret;
    std::set_intersection(begin(), end(), other.begin(), other.end(),
                          std::inserter(ret, ret.begin()));
    return ret;
}

std::size_t AttrIndexesHasher::operator()(AttrIndexes const& a) const {
    return std::hash<std::string>()(join<int>(a, ","));
}

bool Result::operator<(const Result &r) const {
    int i;
    for(i = 0; i < x.size() && i < r.x.size(); ++i) {
        if (x[i] < r.x[i])
            return true;
        else if (x[i] > r.x[i])
            return false;
    }
    if(i < r.x.size())
        return true;
    for(i = 0; i < y.size() && i < r.y.size(); ++i) {
        if (y[i] < r.y[i])
            return true;
        else if (y[i] > r.y[i])
            return false;
    }
    if(i < r.y.size())
        return true;
    return false;
}

std::ostream& operator<<(std::ostream &out, const Result &r) {
    std::vector<int> incx, incy;
    for (auto i : r.x)
        incx.push_back(i + 1);
    for (auto i : r.y)
        incy.push_back(i + 1);
    out << join<int>(incx, " ") << " -> " << join<int>(incy, " ") << std::endl;
    return out;
}

TANE::TANE(Table &t): table(t) {
    for (int i = 0; i < t[0].size(); ++i)
        R.insert_into(i);
}

void TANE::output_result(std::ostream& out) {
    std::sort(results.begin(), results.end());
    for (auto i : results)
        out << i;
}

void TANE::compute() {
    AttrIndexes empty;
    RHS_plus[empty] = R;
    Level L;
    L.l = 0;
    for(auto a : R) {
        AttrIndexes a_set;
        a_set.insert_into(a);
        L.l_l.insert(a_set);
    }
    for (auto i : R) {
        AttrIndexes i_index;
        i_index.insert_into(i);
        partition_map[i_index] = getPartition(i, table);
    }
    while (!L.l_l.empty()) {
        compute_dependencies(L);
        prune(L);
        L = generate_next_level(L);
    }
}

void TANE::compute_dependencies(Level &l) {
    for (auto x: l.l_l) {
        AttrIndexes temp;
        bool first = true;
        for(auto a : x) {
            AttrIndexes x_except_a, a_set;
            a_set.insert_into(a);
            x_except_a = x.difference(a_set);
            if(first) {
                temp = RHS_plus[x_except_a];
                first = false;
            }
            else
                temp = temp.intersection(RHS_plus[x_except_a]);
        }
        RHS_plus[x] = temp;
    }
    for (auto x: l.l_l) {
        AttrIndexes RHS_x_intersect_x = RHS_plus[x].intersection(x);
        for(auto a : RHS_x_intersect_x) {
            AttrIndexes a_set;
            a_set.insert_into(a);
            if(determine(x.difference(a_set), a_set)) {
                results.push_back(Result(x.difference(a_set), a_set));
                RHS_plus[x] = RHS_plus[x].difference(a_set);
                RHS_plus[x] = RHS_plus[x].difference(R.difference(x));
            }
        }
    }
}

void TANE::prune(Level &l) {
    Level new_l;
    for(auto x:l.l_l) {
        if(!RHS_plus[x].empty())
            new_l.l_l.insert(x);
    }
    l.l_l = new_l.l_l;
}

Level TANE::generate_next_level(const Level &L) {
    Level L_n;
    L_n.l = L.l + 1;
    for (auto y = L.l_l.begin(); y != L.l_l.end(); ++y) {
        for (auto z = y; z != L.l_l.end(); ++z) {
            if (z == y)
                continue;
            if (is_prefix(*y, *z)) {
                AttrIndexes x = y -> merge(*z);
                bool flag = true;
                for(auto a : x) {
                    AttrIndexes x_except_a, a_set;
                    a_set.insert_into(a);
                    x_except_a = x.difference(a_set);
                    if(L.l_l.find(x_except_a) == L.l_l.end()) {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                    L_n.l_l.insert(x);
            }
        }
    }
    return L_n;
}

bool TANE::is_prefix(const AttrIndexes& a, const AttrIndexes& b) {
    for (int i = 0; i < a.size() - 1; ++i) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool TANE::determine(const AttrIndexes& x, const AttrIndexes& y) {
    AttrIndexes x_union_y = x.merge(y);
    if (partition_map.find(x_union_y) == partition_map.end()) {
        partition_map[x_union_y] = getPartition(partition_map[x], partition_map[y], table);
    }
    if(partition_map[x_union_y].size == partition_map[x].size)
        return true;
    return false;
}
