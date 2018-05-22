//
// Created by chehang on 2018/5/18.
//

#ifndef FD_TANE_H
#define FD_TANE_H

#include "table/table.h"
#include <unordered_map>
#include <unordered_set>

class AttrIndexes : public std::vector<int>{
public:
    AttrIndexes() = default;
    void insert_into(int new_index);
    AttrIndexes merge(const AttrIndexes& other) const;
    AttrIndexes difference(const AttrIndexes& other) const;
    AttrIndexes intersection(const AttrIndexes& other) const;
};

struct AttrIndexesHasher {
    std::size_t operator()(AttrIndexes const& s) const;
};

struct Level {
    std::unordered_set <AttrIndexes, AttrIndexesHasher> l_l;
    int l;

    Level() = default;
};

class Result {
public:
    AttrIndexes x;
    AttrIndexes y;

    Result(const AttrIndexes& x, const AttrIndexes& y): x(x), y(y) {}
    bool operator<(const Result &r) const;
    friend std::ostream& operator<<(std::ostream& out, const Result& r);
};

class TANE {
    typedef std::vector <std::vector<int>> EquivalentRows;

public:
    TANE (Table& t);
    void compute();
    void output_result(std::ostream& out);

private:
    Table& table;
    std::unordered_map <AttrIndexes, AttrIndexes, AttrIndexesHasher> RHS_plus;
    AttrIndexes R;
    std::vector <Result> results;
    std::unordered_map <AttrIndexes, EquivalentRows,  AttrIndexesHasher> attr_to_eqclasses;

    void compute_dependencies(Level& l);
    void prune(Level& l);
    Level generate_next_level(const Level& L);
    bool is_prefix(const AttrIndexes& a, const AttrIndexes& b);
    bool determine(const AttrIndexes& x, const AttrIndexes& y);

    // Get equivalence classes on attributes `indexes`(each class contain at least two elements)
    EquivalentRows compute_eq_rows(const AttrIndexes& indexes);
};


#endif //FD_TANE_H
