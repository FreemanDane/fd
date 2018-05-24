//
// Created by freeman on 5/19/18.
//

#include "Partition.h"
#include <map>
#include <string>

using std::map;
using std::pair;
using std::make_pair;
using std::string;

Partition getPartition(int pos, const Table & tbl) {
    map<string, int> keys;
    int nrow = tbl.size();
    Partition result;
    int *t = new int[nrow];
    int num_type = 0;
    for (int i = 0; i < nrow; ++i) {
        Data d = tbl[i][pos];
        auto v = keys.find(d);
        if (v == keys.end()) {
            keys.insert(make_pair(d, num_type));
            t[i] = num_type;
            num_type++;
        }
        else {
            t[i] = v->second;
        }
    }
    vector<int> *vts = new vector<int>[num_type];
    for (int i = 0; i < nrow; ++i) {
        vts[t[i]].push_back(i);
    }
    for (int i = 0; i < num_type; ++i) {
        if (vts[i].size() > 1) {
            result.parts.push_back(vts[i]);
        }
    }
    result.size = num_type;
    delete [] t;
    delete [] vts;
    return result;
}

void getPartition(int pos, const Table & tbl, Partition& ret) {
    map<string, int> keys;
    int nrow = tbl.size();
    int *t = new int[nrow];
    int num_type = 0;
    for (int i = 0; i < nrow; ++i) {
        Data d = tbl[i][pos];
        auto v = keys.find(d);
        if (v == keys.end()) {
            keys.insert(make_pair(d, num_type));
            t[i] = num_type;
            num_type++;
        }
        else {
            t[i] = v->second;
        }
    }
    vector<int> *vts = new vector<int>[num_type];
    for (int i = 0; i < nrow; ++i) {
        vts[t[i]].push_back(i);
    }
    for (int i = 0; i < num_type; ++i) {
        if (vts[i].size() > 1) {
            ret.parts.push_back(vts[i]);
        }
    }
    ret.size = num_type;
    delete [] t;
    delete [] vts;
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

void getPartition(const Partition & p1, const Partition & p2, int table_size, Partition &ret) {
    vector<vector<int>> v;
    int l = p1.parts.size();
    int nrow = table_size;
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
                ret.parts.push_back(v[t[j]]);
            }
            v[t[j]].clear();
        }
    }
    delete [] t;
    int hasCom = 0;
    for (auto r : ret.parts) {
        hasCom += r.size();
    }
    ret.size = nrow - hasCom + ret.parts.size();
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