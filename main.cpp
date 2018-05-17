#include "Table.h"

int main() {
    Table table = readData("test/test_data.txt");
    Table table_t = transpose(table);
    return 0;
}
