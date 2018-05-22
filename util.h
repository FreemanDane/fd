//
// Created by chehang on 2018/5/20.
//

#ifndef FD_UTIL_H
#define FD_UTIL_H

#include <vector>
#include <sstream>

template <typename T>
std::string join(const std::vector<T>& v, const std::string& delimiter) {
    std::string s;
    if (v.size()){
        std::stringstream ss;
        for (int i = 0; i < v.size(); ++i) {
            if (i != 0)
                ss << delimiter;
            ss << v[i];
        }
        s = ss.str();
    } else {
        s = "";
    }
    return s;
}

#endif //FD_UTIL_H
