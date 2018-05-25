//
// Created by chehang on 2018/5/23.
//

#ifndef FD_ATTRINDEXES_H
#define FD_ATTRINDEXES_H

#include <iostream>
#include <iterator>

class AttrIndexes {
public:
    AttrIndexes() : indexes(0) {}
    AttrIndexes(int i) : indexes(i) {}

    void insert_into(int new_index);
    AttrIndexes merge(const AttrIndexes& other) const;
    AttrIndexes difference(const AttrIndexes& other) const;
    AttrIndexes intersection(const AttrIndexes& other) const;
    int index() const { return indexes; }
    bool empty() const { return indexes == 0; }
    int size() const;
    bool operator<(const AttrIndexes &a) const;
    bool operator==(const AttrIndexes &a) const;
    void operator=(const int &i) { this->indexes = i; }

    class iterator
    {
    public:
        typedef iterator self_type;
        typedef int value_type;
        typedef int& reference;
        typedef int* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        iterator(int pos, int index) : pos_(pos), index_(index) { }
        self_type operator++() { self_type i = *this; next(); return i; }
        self_type operator++(int junk) { next(); return *this; }
        reference operator*() { return pos_; }
        bool operator==(const self_type& rhs) { return pos_ == rhs.pos_ && index_ == rhs.index_; }
        bool operator!=(const self_type& rhs) { return pos_ != rhs.pos_ && index_ == rhs.index_; }
    private:
        int pos_;
        int index_;
        void next() {
            if(pos_ == 8 * sizeof(int))
                return;
            for(++pos_; pos_ < 8 * sizeof(int); ++pos_) {
                if(index_ & 1 << pos_)
                    return;
            }
        }
    };

    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef int value_type;
        typedef int& reference;
        typedef int* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        const_iterator(int pos, int index) : pos_(pos), index_(index) { }
        self_type operator++() { self_type i = *this; next(); return i; }
        self_type operator++(int junk) { next(); return *this; }
        const reference operator*() { return pos_; }
        bool operator==(const self_type& rhs) { return pos_ == rhs.pos_ && index_ == rhs.index_; }
        bool operator!=(const self_type& rhs) { return pos_ != rhs.pos_ && index_ == rhs.index_; }
    private:
        int pos_;
        int index_;
        void next() {
            if(pos_ == 8 * sizeof(int))
                return;
            for(++pos_; pos_ < 8 * sizeof(int); ++pos_) {
                if(index_ & 1 << pos_)
                    return;
            }
        }
    };

    iterator begin() {
        int pos = 0;
        for (pos; pos < 8 * sizeof(int); ++pos) {
            if(indexes & 1 << pos)
                break;
        }
        return iterator(pos, indexes);
    }

    iterator end() {
        return iterator(8 * sizeof(int), indexes);
    }

    const_iterator begin() const {
        int pos = 0;
        for (pos; pos < 8 * sizeof(int); ++pos) {
            if(indexes & 1 << pos)
                break;
        }
        return const_iterator(pos, indexes);
    }

    const_iterator end() const {
        return const_iterator(8 * sizeof(int), indexes);
    }
private:
    int indexes;
};


struct AttrIndexesHasher {
    std::size_t operator()(AttrIndexes const& s) const;
};


#endif //FD_ATTRINDEXES_H
