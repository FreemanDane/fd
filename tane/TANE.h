//
// Created by chehang on 2018/5/18.
//

#ifndef FD_TANE_H
#define FD_TANE_H

#include "../table/table.h"
#include "../table/Partition.h"
#include "../table/AttrIndexes.h"
#include <unordered_map>
#include <unordered_set>


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
    bool operator==(const Result &r) const;
    friend std::ostream& operator<<(std::ostream& out, const Result& r);
};

class TANE {

public:
    TANE (Table& t);
    void compute();
    void output_result(std::ostream& out);

private:
    Table& table;
    std::unordered_map <AttrIndexes, AttrIndexes, AttrIndexesHasher> RHS_plus;
    AttrIndexes R;
    std::vector <Result> results;
    std::unordered_map <AttrIndexes, Partition, AttrIndexesHasher> partition_map;

    void compute_dependencies(const Level& l);
    void prune(Level& l);
    Level generate_next_level(const Level& L);
    bool is_prefix(const AttrIndexes& a, const AttrIndexes& b);
    bool determine(const AttrIndexes& x, const AttrIndexes& y);
};


#endif //FD_TANE_H
