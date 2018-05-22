//
// Created by freeman on 5/19/18.
//

#include "Partition.h"
#include <map>

using std::multimap;
using std::pair;
using std::make_pair;

Partition getPartition(int pos, const Table & tbl) {
    multimap<Data, int> m;
    vector<Data> keys;
    size_t num_data = tbl.size();
    for (int i = 0; i < num_data; ++i) {
        Data d = tbl[i][pos];
        if (m.find(d) == m.end()) {
            keys.push_back(d);
        }
        m.insert(make_pair(d, i));
    }
    Partition result;
    for (Data & key : keys) {
        vector<int> positions;
        if (m.count(key) == 1) {
            continue;
        } else {
            auto ranges = m.equal_range(key);
            for (auto r = ranges.first; r != ranges.second; ++r) {
                positions.push_back(r->second);
            }
            result.parts.push_back(positions);
        }
    }
    result.size = keys.size();
    return result;
}

Partition getPartition(const Partition & p1, const Partition & p2, const Table & tbl) {
    vector<vector<int>> v1 = p1.parts, v2 = p2.parts;
    Partition result;
    size_t s = 0;
    for (auto vv1 : v1) {
        for (auto vv2 : v2) {
            vector<int> positions;
            auto iter1 = vv1.begin();
            auto iter2 = vv2.begin();
            while (iter1 != vv1.end() && iter2 != vv2.end()) {
                if (*iter1 < *iter2)
                    ++iter1;
                else if (*iter1 > *iter2)
                    ++iter2;
                else if (*iter1 == *iter2) {
                    positions.push_back(*iter1);
                    ++iter1;
                    ++iter2;
                }
            }
            if (positions.size() > 1) {
                result.parts.push_back(positions);
                s += positions.size();
            }
        }
    }
    size_t num_data = tbl.size();
    result.size = result.parts.size() + num_data - s;
    return result;
}

bool operator == (const Partition & p1, const Partition & p2){
    if (p1.size != p2.size) {
        return false;
    }
    if (p1.parts.size() != p2.parts.size()) {
        return false;
    }
    for (auto pt1 : p1.parts) {
        bool find = false;
        for (auto pt2 : p2.parts) {
            if (pt1.size() != pt2.size()) {
                continue;
            }
            if (pt1[0] != pt2[0]) {
                continue;
            }
            for (int i = 0; i < pt1.size(); ++i) {
                if (pt1[i] != pt2[i]) {
                    return false;
                }
            }
            find = true;
        }
        if (!find) {
            return false;
        }
    }
    return true;
}