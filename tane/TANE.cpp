//
// Created by chehang on 2018/5/18.
//

#include "TANE.h"
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <thread>
#include <fstream>
#include "../util.h"


bool Result::operator<(const Result &r) const {
    if (x < r.x)
        return true;
    else if (x.index() == r.x.index() && y < r.y)
        return true;
    return false;
}

bool Result::operator==(const Result &r) const {
    return x == r.x && y == r.y;
}

std::ostream& operator<<(std::ostream &out, const Result &r) {
    std::vector<int> incx, incy;
    for (auto i : r.x)
        incx.push_back(i + 1);
    for (auto i : r.y)
        incy.push_back(i + 1);
    out << join<int>(incx, " ") << " -> " << join<int>(incy, " ") << std::endl;
    return out;
}

TANE::TANE(Table &t, int thread_num): table(t), thread_num(thread_num) {
    for (int i = 0; i < t[0].size(); ++i)
        R.insert_into(i);
}

void TANE::output_result(std::ostream& out) {
    std::sort(results.begin(), results.end());
    for (auto i : results)
        out << i;
}

void TANE::compute() {
    AttrIndexes empty;
    RHS_plus[empty] = R;
    Level L;
    L.l = 0;
    for(auto a : R) {
        AttrIndexes a_set(1 << a);
        L.l_l.insert(a_set);
        partition_map[a_set] = getPartition(a, table);
    }
    while (!L.l_l.empty()) {
        compute_dependencies(L);
        prune(L);
        L = generate_next_level(L);
    }
}

void TANE::compute_dependencies(const Level &l) {
    for (auto x: l.l_l) {
        AttrIndexes temp;
        AttrIndexes x_except_a(x.index() ^ (1 << *x.begin()));
        temp = RHS_plus[x_except_a];
        for(auto a = std::next(x.begin()); a != x.end(); ++a) {
            x_except_a = x.index() ^ (1 << *a);
            temp = temp.intersection(RHS_plus[x_except_a]);
        }
        RHS_plus[x] = temp;
    }
    if(l.l_l.begin()->size() == 1)
        return;
    for (auto x: l.l_l) {
        AttrIndexes RHS_x_intersect_x = RHS_plus[x].intersection(x);
        for(auto a : RHS_x_intersect_x) {
            AttrIndexes a_set(1 << a);
            if(determine(x.difference(a_set), a_set)) {
                results.push_back(Result(x.difference(a_set), a_set));
                RHS_plus[x] = RHS_plus[x].difference(a_set);
                RHS_plus[x] = RHS_plus[x].difference(R.difference(x));
            }
        }
    }
}

void TANE::prune(Level &l) {
    Level new_l;
    for(auto x:l.l_l) {
        if(!RHS_plus[x].empty())
            new_l.l_l.insert(x);
    }
    l.l_l = new_l.l_l;
}

Level TANE::generate_next_level(const Level &L) {
    Level L_n;
    L_n.l = L.l + 1;
    for (auto y = L.l_l.begin(); y != L.l_l.end(); ++y) {
        for (auto z = y; z != L.l_l.end(); ++z) {
            if (z == y)
                continue;
            if (is_prefix(*y, *z)) {
                AttrIndexes x = y -> merge(*z);
                bool flag = true;
                for(auto a : x) {
                    AttrIndexes x_except_a(x.index() ^ (1 << a));
                    if(L.l_l.find(x_except_a) == L.l_l.end()) {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                    L_n.l_l.insert(x);
            }
        }
    }
    return L_n;
}

bool TANE::is_prefix(const AttrIndexes& a, const AttrIndexes& b) {
    auto i = a.begin();
    auto j = b.begin();
    for (; std::next(i) != a.end(); ++i, ++j) {
        if (*i != *j)
            return false;
    }
    return true;
}

bool TANE::determine(const AttrIndexes& x, const AttrIndexes& y) {
    AttrIndexes x_union_y = x.merge(y);
    if (partition_map.find(x_union_y) == partition_map.end()) {
        partition_map[x_union_y] = Partition();
        Partition& x_union_partition = partition_map[x_union_y];
        getPartition(partition_map[x], partition_map[y], table.size(), x_union_partition);
    }
    if(partition_map[x_union_y].size == partition_map[x].size)
        return true;
    return false;
}

void TANE::compute_multithreading() {
    AttrIndexes empty;
    RHS_plus[empty] = R;
    Level L;
    L.l = 0;
    for(auto a : R) {
        AttrIndexes a_set(1 << a);
        L.l_l.insert(a_set);
    }
    TANE::compute_single_partition((void*)this);
    while (!L.l_l.empty()) {
        TANE::compute_dependencies_multithreading(L, (void*)this);
        prune(L);
        L = generate_next_level(L);
    }
}

void TANE::compute_single_partition(void* __this) {
    TANE* _this = (TANE*)__this;
    Missions missions;
    for (auto a : _this->R) {
        AttrIndexes a_set(1 << a);
        missions.push_back(a_set);
        _this->partition_map[a_set] = Partition();
    }
    std::vector<Missions::iterator> m_iter;
    m_iter.push_back(missions.begin());
    int batch_size = missions.size() / (_this->thread_num);
    for(int i = 0; i < _this->thread_num - 1; ++i) {
        m_iter.push_back(m_iter[i] + batch_size);
    }
    m_iter.push_back(missions.end());
    std::vector<std::thread> threads;
    for(int i = 0; i < _this->thread_num; ++i) {
        threads.push_back(std::thread(compute_single_partition_batch, m_iter[i], m_iter[i+1], __this));
    }
    for(int i = 0; i < threads.size(); i++){
        threads[i].join();
    }
}

void TANE::compute_single_partition_batch(TANE::MissionsIter begin, TANE::MissionsIter end, void* __this) {
    TANE* _this = (TANE*)__this;
    for (; begin != end; ++begin) {
        _this->partition_map[*begin] = getPartition(*(begin->begin()), _this->table);
    }
}

void TANE::compute_dependencies_multithreading(const Level &l, void* __this) {
    TANE* _this = (TANE*)__this;
    int thread_num = _this->thread_num;
    for (auto x: l.l_l) {
        AttrIndexes temp;
        AttrIndexes x_except_a(x.index() ^ (1 << *x.begin()));
        temp = _this->RHS_plus[x_except_a];
        for(auto a = std::next(x.begin()); a != x.end(); ++a) {
            x_except_a = x.index() ^ (1 << *a);
            temp = temp.intersection(_this->RHS_plus[x_except_a]);
        }
        _this->RHS_plus[x] = temp;
    }
    if(l.l_l.begin()->size() == 1)
        return;
    Missions missions;
    for (auto a : l.l_l) {
        missions.push_back(a);
        _this->partition_map[a] = Partition();
    }
    std::vector<Missions::iterator> m_iter;
    m_iter.push_back(missions.begin());
    int batch_size = missions.size() / (thread_num);
    for(int i = 0; i < thread_num - 1; ++i) {
        m_iter.push_back(m_iter[i] + batch_size);
    }
    m_iter.push_back(missions.end());
    std::vector<std::thread> threads;
    std::vector<std::vector <Result>> results(thread_num);
    for(int i = 0; i < thread_num; ++i) {
        threads.push_back(std::thread(compute_fd_batch, m_iter[i], m_iter[i+1], __this, std::ref(results[i])));
    }
    for(int i = 0; i < thread_num; ++i) {
        threads[i].join();
    }
    std::vector <Result>& final_result =_this->results;
    for(int i = 0; i < thread_num; ++i) {
        final_result.insert(final_result.end(), results[i].begin(), results[i].end());
    }
}

void TANE::compute_fd_batch(TANE::MissionsIter begin, TANE::MissionsIter end, void* __this, std::vector <Result>& results) {
    TANE* _this = (TANE*)__this;
    for (; begin != end; ++begin) {
        auto x = *begin;
        AttrIndexes RHS_x_intersect_x = _this->RHS_plus[x].intersection(x);
        for(auto a : RHS_x_intersect_x) {
            AttrIndexes a_set(1 << a);
            if(TANE::determine(x.difference(a_set), a_set, _this->table.size(), _this->partition_map)) {
                results.push_back(Result(x.difference(a_set), a_set));
                _this->RHS_plus[x] = _this->RHS_plus[x].difference(a_set);
                _this->RHS_plus[x] = _this->RHS_plus[x].difference(_this->R.difference(x));
            }
        }
    }
}

bool TANE::determine(const AttrIndexes& x, const AttrIndexes& y, int table_size, TANE::PartitionMap& partition_map){
    AttrIndexes x_union_y = x.merge(y);
    if (partition_map[x_union_y].size == 0) {
        partition_map[x_union_y] = Partition();
        Partition& x_union_partition = partition_map[x_union_y];
        getPartition(partition_map[x], partition_map[y], table_size, x_union_partition);
    }
    if(partition_map[x_union_y].size == partition_map[x].size)
        return true;
    return false;
}
