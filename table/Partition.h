//
// Created by fengs on 2018/5/19.
//

#ifndef FD_PARTITION_H
#define FD_PARTITION_H

#include <vector>
#include "Table.h"
using std::vector;

struct Partition
{
    size_t size;
    vector<vector<int>> parts;
};

Partition getPartition(int pos, const Table & tbl);
void getPartition(int pos, const Table & tbl, Partition& ret);
Partition getPartition(const Partition &, const Partition &, const Table & tb);
void getPartition(const Partition & p1, const Partition & p2, int table_size, Partition &ret);
bool operator == (const Partition &, const Partition &);

#endif //FD_PARTITION_H
