//
// Created by chehang on 2018/5/17.
//

#ifndef FD_TABLE_H
#define FD_TABLE_H

#include <vector>
#include <string>

typedef std::string Data;
typedef std::vector<Data> Row;
typedef std::vector<Row> Table;

Table readData(std::string filename);
Table transpose(Table t);

#endif //FD_TABLE_H
