//
// Created by chehang on 2018/5/17.
//

#include "Table.h"

#include <iostream>
#include <fstream>
#include <sstream>


Table readData(std::string filename) {
    std::ifstream in(filename);
    if(!in)
        throw "File not exists";
    Table table;
    std::string line;
    while(std::getline(in, line)) {
        Row row;
        Data data;
        int start = 0, end = 0;
        for(; end < line.size() - 1; ++end) {
            if (line[end] == ',' && line[end + 1] != ' ') {
                row.push_back(line.substr(start, end - start));
                start = end + 1;
            }
        }
        if(end != start)
            row.push_back(line.substr(start, end - start + 1));
        table.push_back(row);
    }
    return table;
}

Table transpose(Table t) {
    Table table_t;
    if (t.size() > 0){
        for (int i = 0; i < t[0].size(); ++i) {
            Row new_row;
            for (int j = 0; j < t.size(); ++j)
                new_row.push_back(t[j][i]);
            table_t.push_back(new_row);
        }
    }
    return table_t;
}