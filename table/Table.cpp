//
// Created by chehang on 2018/5/17.
//

#include "Table.h"

#include <iostream>
#include <fstream>
#include <sstream>


Table readData(std::string filename) {
    std::ifstream in(filename);
    Table table;
    std::string line;
    while(std::getline(in, line)) {
        Row row;
        Data data;
        std::stringstream row_stream(line);
        while(std::getline(row_stream, data, ','))
            row.push_back(data);
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