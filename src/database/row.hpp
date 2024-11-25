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
        // Rows are not default constructible or copyable
        Row()                               = delete;
        Row(const Row& other)               = delete;
        Row& operator= (const Row& other)   = delete;

        // Rows are movable
        Row(Row&& other)            = default;
        Row& operator=(Row&& other) = default;

        Row(Table* table, std::vector<Cell> const &data);
        Row(Table* table, std::vector<Cell>&& data);
        Row(Table* table, std::unordered_map<std::string, Cell> const &data);

        size_t size() const { return data_.size(); }

        Cell& operator[] (size_t index);
        const Cell& operator[] (size_t index) const;

        Table* get_table() const;
    private:
        Table* table_;
        std::vector<Cell> data_;
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_ROW_H