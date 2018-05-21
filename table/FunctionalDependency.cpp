//
// Created by fengs on 2018/5/19.
//

#include "FunctionalDependency.h"
#include <string>

using std::string;

FunctionalDependency::FunctionalDependency(const ColumnCombination & l, const ColumnCombination & r) : left(l), right(r) {}

ofstream & operator << (ofstream & fout, FunctionalDependency fd){
    string result = "";
    size_t len = fd.left.maxsize();
    for (int i = 0; i < len; ++i) {
        if (fd.left[i] == 1) {
            if (result.length() != 0) {
                result += ' ';
            }
            result += ('1' + i);
        }
    }
    result += "->";
    for (int i = 0; i < len; ++i) {
        if (fd.right[i] == 1) {
            if (result.length() != 0) {
                result += ' ';
            }
            result += ('1' + i);
        }
    }
    fout << result;
    return fout;
}

bool operator < (const FunctionalDependency & f1, const FunctionalDependency & f2) {
    auto msize = f1.left.maxsize();
    for (int i = 0; i < msize; ++i) {
        if (f1.left[i] == 0 && f2.left[i] != 0) {
            return false;
        } else if (f1.left[i] != 0 && f2.left[i] == 0) {
            return true;
        }
    }
    for (int i = 0; i < msize; ++i) {
        if (f1.right[i] == 0 && f2.right[i] != 0) {
            return false;
        } else if (f1.right[i] != 0 && f2.right[i] == 0) {
            return true;
        }
    }
    return false;
}

bool operator == (const FunctionalDependency & f1, const FunctionalDependency & f2) {
    return (f1.right.getCombination() == f2.right.getCombination()) && (f1.left.getCombination() == f2.left.getCombination());
}

bool operator > (const FunctionalDependency & f1, const FunctionalDependency & f2) {
    return f2 < f1;
}

bool operator >= (const FunctionalDependency & f1, const FunctionalDependency & f2) {
    return f1 > f2 || f1 == f2;
}

bool operator <= (const FunctionalDependency & f1, const FunctionalDependency & f2) {
    return f1 < f2 || f1 == f2;
}