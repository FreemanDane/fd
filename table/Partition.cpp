//
// Created by freeman on 5/19/18.
//

#include "Partition.h"
#include <map>

using std::map;
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
    Partition result;
    vector<vector<int>> v;
    int l = p1.parts.size();
    int nrow = tbl.size();
    int *t = new int[nrow];
    for (int i = 0; i < nrow; ++i) {
        t[i] = -1;
    }
    for (int i = 0; i < l; ++i) {
        for (auto j : p1.parts[i]) {
            t[j] = i;
        }
        vector<int> vv;
        v.push_back(vv);
    }
    l = p2.parts.size();
    for (int i = 0; i < l; ++i) {
        for (auto j : p2.parts[i]) {
            if (t[j] != -1 || t[j] < v.size()) {
                v[t[j]].push_back(j);
            }
        }
        for (auto j : p2.parts[i]) {
            if (t[j] >= v.size())
                continue;
            if (v[t[j]].size() > 1) {
                result.parts.push_back(v[t[j]]);
            }
            v[t[j]].clear();
        }
    }
    delete [] t;
    int hasCom = 0;
    for (auto r : result.parts) {
        hasCom += r.size();
    }
    result.size = nrow - hasCom + result.parts.size();
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