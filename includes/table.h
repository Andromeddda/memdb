#ifndef    HEADER_GUARD_TABLE_H
#define HEADER_GUARD_TABLE_H

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <variant>
#include <unordered_map>

#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <cstdint>

#define MAX_TABLE_NAME  128U
#define MAX_COLUMN_NAME 128U
#define MAX_STRING_DATA 256U

#define COLUMN_ATTRIBUTE_KEY            1U
#define COLUMN_ATTRIBUTE_UNIQUE         2U
#define COLUMN_ATTRIBUTE_AUTOINCREMENT  4U

namespace memdb 
{
    enum ColumnType {
        ColumnTypeInt32,
        ColumnTypeBool,
        ColumnTypeString,
        ColumnTypeBytes
    };

    // Types of data stored in table cell
    using Int32     = uint32_t;
    using Bool      = bool;
    using String    = std::array<signed   char, MAX_STRING_DATA>;
    using Bytes     = std::array<unsigned char, MAX_STRING_DATA>;

    struct ColumnDescription {
        const ColumnType      type_;
        const std::string     name_;
        const unsigned char   attributes_;

        ColumnDescription(
            ColumnType type, 
            const char* name, 
            unsigned char attributes);

        ColumnDescription(
            ColumnType type, 
            const std::string& name, 
            unsigned char attributes);

        ~ColumnDescription() = default;
    };

    // Dynamic type of cell data, stored as type-safe union 
    typedef
        typename std::variant<Int32, Bool, String, Bytes>
        cell_t;

    using row_t = typename std::vector<cell_t>;

    class Table 
    {
    public:
        Table() = delete;
        Table(const std::string& name);
        Table(const char* name);

        Table(Table&& other) = default;
        Table(const Table& other) = default;

        Table& operator= (Table&& other) = default;
        Table& operator= (const Table& other) = default;

    private:
        const char*
            name_;            // Table name

        std::vector<ColumnDescription>
            columns_;         //  List of column descriptions (type, name and attributes)

        std::vector<row_t> 
            rows_;             // List of rows

        std::unordered_map<cell_t, size_t>
            index_;         // Map that associates cell data with row number
    };
} // namespace memdb

#endif // HEADER_GUARD_TABLE_H