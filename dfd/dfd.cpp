//
// Created by fengs on 2018/5/20.
//

#include "dfd.h"
#include <fstream>
#include <stack>
#include <iostream>
using std::string;
using std::ofstream;
using std::stack;
using std::cout;

stack<ColumnCombination> nodeStack;

vector<FunctionalDependency> dfdmain(const string & filename) {
    Table tbl = readData(filename);
    int num_col = tbl[0].size();
    int num_com = 1 << num_col;
    vector<FunctionalDependency> result;
    ColumnCombination total(num_com - 1, &tbl);
    Partition *parts = new Partition[num_com];
    for (int i = 0; i < num_com; ++i) {
        parts[i].size = 0;
    }
    for (int i = 0; i < num_col; ++i) {
        int num = 1 << i;
        parts[num] = getPartition(i, tbl);
    }
    cout << "Compute single partition finally.\n";
    for (int i = 0; i < num_col; ++i) {
        int com1 = 1 << i;
        bool unique = true;
        for (int j = 0; j < num_col; ++j) {
            int com2 = 1 << j;
            if (com1 == com2)
                continue;
            if (parts[com1 + com2].size == 0) {
                parts[com1 + com2] = getPartition(parts[com1], parts[com2], tbl);
            }
            if (parts[com1].size != parts[com1 + com2].size)
                unique = false;
        }
        if (unique) {
            total.remove(i);
            for (int j = 0; j < num_col; ++j) {
                if (i == j) {
                    continue;
                }
                result.push_back(FunctionalDependency(ColumnCombination(1 << i, &tbl), ColumnCombination(1 << j, &tbl)));
            }
        }
    }
    cout << "Find unique column finally.\n";
    for (int i = 0; i < num_col; ++i) {
        ColumnCombination RHS = ColumnCombination(1 << i, &tbl);
        auto LHSs = findLHSs(RHS, total, parts);
        cout << "find LHSs of " << i + 1 << std::endl;
        for (auto LHS : LHSs) {
            result.push_back(FunctionalDependency(LHS, RHS));
        }
    }
    delete [] parts;
    return result;
}

vector<ColumnCombination> findLHSs(ColumnCombination RHS, ColumnCombination total, Partition *parts) {
    Table *tbl = total.getTable();
    int num_col = (*tbl)[0].size();
    int num_com = 1 << num_col;
    Category *ctg = new Category[num_com];
    for (int i = 0; i < num_com; ++i) {
        ctg[i] = NOT_VISITED;
    }
    //ctg[0] = static_cast<Category >(CANDIDATE | MAX_OR_MIN);
    vector<ColumnCombination> seeds;
    for (int i = 0; i < num_col; ++i) {
        if (total[i] == 1 && RHS[i] == 0) {
            seeds.push_back(ColumnCombination(1 << i, tbl));
        }
    }
    vector<ColumnCombination> minDeps;
    vector<ColumnCombination> maxNonDeps;
    while (seeds.size() != 0) {
        ColumnCombination node(seeds[0]);
        do {
            int v = node.getCombination();
            if (ctg[v] != NOT_VISITED) {
                if ((ctg[v] & CANDIDATE) != 0) {
                    if ((ctg[v] & DEPENDENCY) != 0) {
                        if (isMinimal(node, RHS, total, ctg, num_col)) {
                            minDeps.push_back(node);
                        }
                    } else {
                        if (isMaximal(node, RHS, total, ctg, num_col)) {
                            maxNonDeps.push_back(node);
                        }
                    }
                }
            } else {
                inferCategory(node, RHS, total, ctg, num_col);
                if (ctg[node.getCombination()] == NONE) {
                    computePartition(node, RHS, parts, ctg, num_col);
                }
            }
            node = pickNextNode(node, RHS, total, seeds, ctg, minDeps, maxNonDeps, num_col);
        }while (node.size() != -1);
        seeds = generateNextSeeds(minDeps, maxNonDeps, total, RHS, num_col, tbl);
        //seeds.clear();
    }
    delete [] ctg;
    return minDeps;
}

bool isMinimal(const ColumnCombination & left, const ColumnCombination & right, const ColumnCombination & total, Category *ctg, int num_col) {
    for (int i = 0; i < num_col; ++i) {
        if (left[i] != 1 || right[i] == 1 || total[i] == 0) {
            continue;
        }
        int index = 1 << i;
        Category c = ctg[left.getCombination() - index];
        if (c == NOT_VISITED || c == NONE)
            return false;
        if ((c & DEPENDENCY) != 0) {
            ctg[left.getCombination()] = DEPENDENCY;
            return false;
        }
    }
    ctg[left.getCombination()] = static_cast<Category>(DEPENDENCY | MAX_OR_MIN);
    return true;
}

bool isMaximal(const ColumnCombination & left, const ColumnCombination & right, const ColumnCombination & total, Category *ctg, int num_col) {
    for (int i = 0; i < num_col; ++i) {
        if (left[i] == 1 || right[i] == 1 || total[i] == 0) {
            continue;
        }
        int index = 1 << i;
        Category c = ctg[left.getCombination() + index];
        if (c == NOT_VISITED || c == NONE)
            return false;
        if ((c & DEPENDENCY) == 0) {
            ctg[left.getCombination()] = static_cast<Category>(0);
            return false;
        }
    }
    ctg[left.getCombination()] = MAX_OR_MIN;
    return true;
}

void inferCategory(const ColumnCombination & left, const ColumnCombination & right, const ColumnCombination & total, Category *ctg, int num_col){
    for (int i = 0; i < num_col; ++i) {
        if (left[i] != 1 || right[i] == 1 || total[i] == 0)
            continue;
        int index = 1 << i;
        Category c = ctg[left.getCombination() - index];
        if (c == NONE || c == NOT_VISITED)
            continue;
        if ((c & DEPENDENCY) != 0) {
            ctg[left.getCombination()] = DEPENDENCY;
            return;
        }
    }
    for (int i = 0; i < num_col; ++i) {
        if (left[i] == 1 || right[i] == 1 || total[i] == 0)
            continue;
        int index = 1 << i;
        Category c = ctg[left.getCombination() + index];
        if (c == NONE || c == NOT_VISITED)
            continue;
        if ((c & DEPENDENCY ) == 0) {
            ctg[left.getCombination()] = static_cast<Category>(0);
            return;
        }
    }
    ctg[left.getCombination()] = NONE;
}

void getPartition(const ColumnCombination & target, Partition *parts, int num_col) {
    Table *tbl = target.getTable();
    int num_com = 1 << num_col;
    for (int i = 1; i < num_com; ++i) {
        int t1 = 1 << i;
        ColumnCombination c1(t1, tbl);
        if (!c1.isSubset(target))
            continue;
        int t2 = target.getCombination() - c1.getCombination();
        if (parts[t1].size != 0 && parts[t2].size != 0) {
            parts[target.getCombination()] = getPartition(parts[t1], parts[t2], *tbl);
            return;
        }
    }
    Partition p;
    p.size = 0;
    for (int i = 0; i < num_col; ++i) {
        if (target[i] == 0)
            continue;
        int index = 1 << i;
        if (p.size == 0)
            p = parts[index];
        else
            p = getPartition(p, parts[index], *tbl);
    }
    parts[target.getCombination()] = p;
}

void computePartition(const ColumnCombination & left, const ColumnCombination & right, Partition *parts, Category *ctg, int num_col){
    Partition *leftp, *totalp;
    leftp = &parts[left.getCombination()];
    totalp = &parts[left.getCombination() + right.getCombination()];
    if (leftp->size == 0) {
        getPartition(left, parts, num_col);
        leftp = &parts[left.getCombination()];
    }
    if (totalp->size == 0) {
        ColumnCombination tt = left + right;
        getPartition(tt, parts, num_col);
        totalp = &parts[tt.getCombination()];
    }
    if (leftp->size == totalp->size) {
        ctg[left.getCombination()] = static_cast<Category>(CANDIDATE | DEPENDENCY | MAX_OR_MIN);
    } else {
        ctg[left.getCombination()] = static_cast<Category >(CANDIDATE | MAX_OR_MIN);
    }
}

ColumnCombination pickNextNode(const ColumnCombination & node, const ColumnCombination & RHS, const ColumnCombination & total, vector<ColumnCombination> seeds, Category *ctg, vector<ColumnCombination> &minDep, vector<ColumnCombination> &maxNonDep, int num_col) {
    vector<ColumnCombination> s,p;
    int com = node.getCombination();
    Table *tbl = node.getTable();
    ColumnCombination nextNode(tbl);
    if (ctg[com] == (DEPENDENCY | CANDIDATE | MAX_OR_MIN)) {
        for (int i = 0; i < num_col; ++i) {
            if (node[i] != 1 || RHS[i] == 1 || total[i] == 0)
                continue;
            int v = com - (1 << i);
            if (ctg[v] == NONE || ctg[v] == NOT_VISITED) {
                nodeStack.push(node);
                return ColumnCombination(v, tbl);
            }
        }
        minDep.push_back(node);
    }
    else if (ctg[com] == (CANDIDATE | MAX_OR_MIN)) {
        for (int i = 0; i < num_col; ++i) {
            if (node[i] == 1 || RHS[i] == 1 || total[i] == 0)
                continue;
            int v = com + (1 << i);
            if (ctg[v] == NONE || ctg[v] == NOT_VISITED) {
                nodeStack.push(node);
                return ColumnCombination(v, tbl);
            }
        }
        maxNonDep.push_back(node);
    }
    if (nodeStack.size() == 0)
        return ColumnCombination(tbl);
    else {
        nextNode = nodeStack.top();
        nodeStack.pop();
        return nextNode;
    }
}

vector<ColumnCombination> generateNextSeeds(vector<ColumnCombination> &minDep, vector<ColumnCombination> &maxNonDep, const ColumnCombination & total, const ColumnCombination & target, int num_col, Table *tbl) {
    vector<ColumnCombination> seeds, newSeeds;
    for (auto mdp : maxNonDep) {
        ColumnCombination cmdp = mdp.complement();
        cmdp = cmdp / target;
        cmdp = cmdp * total;
        if (seeds.size() == 0) {
            for (int i = 0; i < num_col; ++i) {
                if (cmdp[i] == 1 && total[i] != 0) {
                    seeds.push_back(ColumnCombination(1 << i, tbl));
                }
            }
        }
        else {
            for (auto dep : seeds) {
                for (int i = 0; i < num_col; ++i) {
                    if (cmdp[i] == 1 && total[i] != 0) {
                        ColumnCombination cc(dep);
                        cc.add(i);
                        newSeeds.push_back(cc);
                    }
                }
            }
            for (auto p = newSeeds.begin(); p != newSeeds.end();){
                bool erase = false;
                for (auto q = newSeeds.begin(); q != newSeeds.end(); ++q) {
                    if (p == q)
                        continue;
                    if (p->isSuperset(*q)) {
                        p = newSeeds.erase(p);
                        erase = true;
                        break;
                    }
                }
                if (!erase) {
                    p++;
                }
            }
            seeds.clear();
            for (auto ns : newSeeds) {
                seeds.push_back(ns);
            }
            newSeeds.clear();
        }
    }
    for (auto p = seeds.begin(); p != seeds.end();) {
        bool found = false;
        for (auto q = minDep.begin(); q != minDep.end(); ++q) {
            if (*p == *q) {
                found = true;
                p = seeds.erase(p);
                break;
            }
        }
        if (!found)
            p++;
    }
    return seeds;
}