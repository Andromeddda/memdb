#ifndef HEADER_GUARD_DATABASE_ROW_H
#define HEADER_GUARD_DATABASE_ROW_H

#include <vector>
#include <string>
#include <unordered_map>

#include "cell/cell.hpp"

namespace memdb
{
    class Table;

    class Row
    {
    public:
        Row() = delete;

        Row(Table* table, const std::vector<Cell>& data);
        Row(Table* table, std::vector<Cell>&& data);
        Row(Table* table, const std::unordered_map<std::string, Cell>& data);

        Cell& operator[] (size_t index);
        const Cell& operator[] (size_t index) const;

        Table* get_table() const;
    private:
        Table* table_;
        std::vector<Cell> data_;
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_ROW_H