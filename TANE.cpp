//
// Created by chehang on 2018/5/18.
//

#include "TANE.h"
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "util.h"


//#define COUNT_CALCULATE


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

    EquivalentRows equivalence_classes;
    std::vector <int> all_rows;
    for (int i = 0; i < table.size(); ++i)
        all_rows.push_back(i);
    equivalence_classes.push_back(all_rows);
    attr_to_eqclasses[empty] = equivalence_classes;

    while (!L.l_l.empty()) {
#ifdef COUNT_CALCULATE
        std::cout << L.l_l.size() << "\t" << L.l_l.begin() -> size() << std::endl;
#endif
        compute_dependencies(L);
        prune(L);
#ifdef COUNT_CALCULATE
        std::cout << L.l_l.size() << "\t" << L.l_l.begin() -> size() << std::endl;
#endif
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
#ifdef COUNT_CALCULATE
    int count = 0;
#endif
    for (auto x: l.l_l) {
        AttrIndexes RHS_x_intersect_x = RHS_plus[x].intersection(x);
        for(auto a : RHS_x_intersect_x) {
            AttrIndexes a_set;
            a_set.insert_into(a);
#ifdef COUNT_CALCULATE
            ++count;
#endif
            if(determine(x.difference(a_set), a_set)) {
                results.push_back(Result(x.difference(a_set), a_set));
                RHS_plus[x] = RHS_plus[x].difference(a_set);
                RHS_plus[x] = RHS_plus[x].difference(R.difference(x));
            }
        }
    }
#ifdef COUNT_CALCULATE
    std::cout << count << std::endl;
#endif
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

bool TANE::determine(const AttrIndexes& x, const AttrIndexes& y){
    if (x.empty())
        return false;
    EquivalentRows equivalence_classes;
    equivalence_classes = compute_eq_rows(x);
    for (auto equivalence_class : equivalence_classes) {
        for (auto i = equivalence_class.begin(); i != std::prev(equivalence_class.end()); ++i) {
            int row = *i, next_row = *std::next(i);
            for (auto j : y) {
                if (table[row][j] != table[next_row][j])
                    return false;
            }
        }
    }
    return true;
}

TANE::EquivalentRows TANE::compute_eq_rows(const AttrIndexes& indexes) {
    int prefix_size = indexes.size() - 1;
    AttrIndexes prefix = indexes;
    EquivalentRows equivalence_classes;
    while(prefix_size >= 0) {
        prefix.resize(prefix_size);
        if (attr_to_eqclasses.find(prefix) != attr_to_eqclasses.end()) {
            equivalence_classes = attr_to_eqclasses[prefix];
            break;
        }
        --prefix_size;
    }
    for (int i = prefix_size; i < indexes.size(); ++i) {
        std::vector <std::vector<int>> equivalence_classes_next;
        for (auto equivalence_class : equivalence_classes) {
            std::unordered_map <std::string, std::vector<int>> row_map;
            for (auto row : equivalence_class)
                row_map[table[row][indexes[i]]].push_back(row);
            for (auto j : row_map) {
                if (j.second.size() > 1)
                    equivalence_classes_next.push_back(j.second);
            }
        }
        equivalence_classes = equivalence_classes_next;
        prefix = indexes;
        prefix.resize(i + 1);
        attr_to_eqclasses[prefix] = equivalence_classes;
    }
    return equivalence_classes;
}
