//
// Created by fengs on 2018/5/19.
//

#ifndef FD_PARTITIONM_H
#define FD_PARTITIONM_H

#include <vector>
#include <mutex>
#include "Table.h"

struct PartitionM
{
    size_t size;
    std::vector<std::vector<int>> parts;
    std::mutex m;
    PartitionM() = default;
    PartitionM(PartitionM & p) {
        size = p.size;
        parts = p.parts;
    }
    PartitionM(PartitionM && p) {
        size = p.size;
        parts = p.parts;
    }
    PartitionM& operator=(PartitionM p) {
        size = p.size;
        parts = p.parts;
        return *this;
    }
};

PartitionM getPartitionM(int pos, const Table & tbl);
void getPartitionM(int pos, const Table & tbl, PartitionM& ret);
PartitionM getPartitionM(const PartitionM &, const PartitionM &, const Table & tb);
void getPartitionM(const PartitionM & p1, const PartitionM & p2, int table_size, PartitionM &ret);
bool operator == (const PartitionM &, const PartitionM &);

#endif