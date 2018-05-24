//
// Created by chehang on 2018/5/18.
//

#ifndef FD_TANE_H
#define FD_TANE_H

#include "../table/Table.h"
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
    TANE (Table& t, int thread_num = 1);
    void compute();
    void compute_multithreading();
    void output_result(std::ostream& out);

private:
    typedef std::unordered_map <AttrIndexes, Partition, AttrIndexesHasher> PartitionMap;
    Table& table;
    std::unordered_map <AttrIndexes, AttrIndexes, AttrIndexesHasher> RHS_plus;
    AttrIndexes R;
    std::vector <Result> results;
    PartitionMap partition_map;
    int thread_num;

    void compute_dependencies(const Level& l);
    void prune(Level& l);
    Level generate_next_level(const Level& L);
    bool is_prefix(const AttrIndexes& a, const AttrIndexes& b);
    bool determine(const AttrIndexes& x, const AttrIndexes& y);

    typedef std::vector<AttrIndexes> Missions;
    typedef Missions::iterator MissionsIter;
    static void compute_single_partition(void* __this);
    static void compute_single_partition_batch(MissionsIter begin, MissionsIter end, void* __this);
    static void compute_fd_batch(MissionsIter begin, MissionsIter end, void* __this, std::vector <Result>& results);
    static bool determine(const AttrIndexes& x, const AttrIndexes& y, int table_size, PartitionMap& partition_map);
    static void compute_dependencies_multithreading(const Level& l, void* __this);
};


#endif //FD_TANE_H
