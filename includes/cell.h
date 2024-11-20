#ifndef HEADER_GUARD_CELL_H
#define HEADER_GUARD_CELL_H

#include <cstddef>
#include <string>
#include <variant>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>

#include <stdint.h>
#include "db_exceptions.h"

#define MAX_STRING_DATA 256U

namespace memdb 
{
    // Data types stored in table cell
    using Int32     = int32_t;
    using Bool      = bool;

    #define CACHE_STRINGS
    #ifdef  CACHE_STRINGS
        using String    = typename std::array<char, MAX_STRING_DATA>;
        using Bytes     = typename std::array<std::byte, MAX_STRING_DATA>;
    #else
        using String    = typename std::string;
        using Bytes     = typename std::vector<std::byte>;
    #endif // CACHE_STRINGS

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
        Cell(const std::string& value);
        Cell(const std::vector<std::byte>& value);

        Cell(const Cell& other) = default;
        Cell(Cell&& other) = default;

        Cell& operator=(const Cell& other) = default;
        Cell& operator=(Cell&& other) = default;

        bool is_int() const;
        bool is_bool() const;
        bool is_string() const;
        bool is_bytes() const;

        ColumnType get_type() const;
        std::string display() const;

        bool less(const Cell& other) const;

        Int32           get_int() const;
        Bool            get_bool() const;
        std::string     get_string() const;
        std::vector<std::byte>   get_bytes() const;

        // Comparison operators
        Cell operator== (const Cell& other) const;
        Cell operator!= (const Cell& other) const;
        Cell operator>= (const Cell& other) const;
        Cell operator<= (const Cell& other) const;
        Cell operator< (const Cell& other) const;
        Cell operator> (const Cell& other) const;

        // Arithmetical operators (For Int32)
        Cell operator- () const;
        Cell operator-(const Cell& other) const;
        Cell operator/ (const Cell& other) const;
        Cell operator% (const Cell& other) const;
        Cell operator* (const Cell& other) const; // Int32 and Bool
        Cell operator+ (const Cell& other) const; // Int32, String and Bool

        // Logical operators (For Bool)
        Cell operator! () const;
        Cell operator&& (const Cell& other) const;
        Cell operator|| (const Cell& other) const;

        // Bitwise operators (For Int32, Bool and Bytes)
        Cell operator~ ();
        Cell operator| (const Cell& other) const;
        Cell operator& (const Cell& other) const;
        Cell operator^ (const Cell& other) const;
        

    private:
        std::variant<Int32, Bool, String, Bytes>
            value_;

        size_t size_;
    };

} // namespace memdb

#endif // HEADER_GUARD_CELL_H