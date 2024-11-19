#ifndef HEADER_GUARD_CELL_H
#define HEADER_GUARD_CELL_H

#include <cstddef>
#include <string>
#include <variant>
#include <iostream>
#include <vector>
#include <array>

#include <stdint.h>
#include "db_exceptions.h"

#define MAX_STRING_DATA 256U

namespace memdb 
{
    // Data types stored in table cell
    using Int32     = int32_t;
    using Bool      = bool;
    using String    = typename std::array<char, MAX_STRING_DATA>;
    using Bytes     = typename std::array<std::byte, MAX_STRING_DATA>;

    // Flags for types of data stored in one table column
    enum ColumnType {
        ColumnTypeInt32,
        ColumnTypeBool,
        ColumnTypeString,
        ColumnTypeBytes
    };

    class Cell
    {
    public:
        Cell() = default;
        Cell(Int32 value);
        Cell(Bool value);
        Cell(std::string& value);
        Cell(Bytes&& value);

        Cell(const Cell& other) = default;
        Cell(Cell&& other) = default;

        Cell& operator=(const Cell& other) = default;
        Cell& operator=(Cell&& other) = default;

        bool is_int() const;
        bool is_bool() const;
        bool is_string() const;
        bool is_bytes() const;

        ColumnType get_type() const;
        bool operator< (const Cell& other) const;

        Int32& get_int();
        Bool& get_bool();
        String& get_string();
        Bytes& get_bytes();

        std::string display() const;
    private:
        std::variant<Int32, Bool, String, Bytes>
            value_;
    };

} // namespace memdb

#endif // HEADER_GUARD_CELL_H