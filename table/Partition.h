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
Partition getPartition(const Partition &, const Partition &, const Table & tb);
bool operator == (const Partition &, const Partition &);

#endif //FD_PARTITION_H
