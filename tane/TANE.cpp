//
// Created by chehang on 2018/5/18.
//

#include "TANE.h"
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "../util.h"


bool Result::operator<(const Result &r) const {
    if (x < r.x)
        return true;
    else if (x.index() == r.x.index() && y < r.y)
        return true;
    return false;
}

bool Result::operator==(const Result &r) const {
    return x == r.x && y == r.y;
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
        AttrIndexes a_set(1 << a);
        L.l_l.insert(a_set);
        partition_map[a_set] = getPartition(a, table);
    }
    while (!L.l_l.empty()) {
        compute_dependencies(L);
        prune(L);
        L = generate_next_level(L);
    }
}

void TANE::compute_dependencies(const Level &l) {
    for (auto x: l.l_l) {
        AttrIndexes temp;
        AttrIndexes x_except_a(x.index() ^ (1 << *x.begin()));
        temp = RHS_plus[x_except_a];
        for(auto a = std::next(x.begin()); a != x.end(); ++a) {
            x_except_a = x.index() ^ (1 << *a);
            temp = temp.intersection(RHS_plus[x_except_a]);
        }
        RHS_plus[x] = temp;
    }
    for (auto x: l.l_l) {
        AttrIndexes RHS_x_intersect_x = RHS_plus[x].intersection(x);
        for(auto a : RHS_x_intersect_x) {
            AttrIndexes a_set(a);
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
                    AttrIndexes x_except_a(x.index() ^ (1 << a));
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
    auto i = a.begin();
    auto j = b.begin();
    for (; std::next(i) != a.end(); ++i, ++j) {
        if (*i != *j)
            return false;
    }
    return true;
}

bool TANE::determine(const AttrIndexes& x, const AttrIndexes& y) {
    AttrIndexes x_union_y = x.merge(y);
    if (partition_map.find(x_union_y) == partition_map.end()) {
        partition_map[x_union_y] = Partition();
        Partition& x_union_partition = partition_map[x_union_y];
        getPartition(partition_map[x], partition_map[y], table.size(), x_union_partition);
    }
    if(partition_map[x_union_y].size == partition_map[x].size)
        return true;
    return false;
}
