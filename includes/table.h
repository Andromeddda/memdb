#ifndef    HEADER_GUARD_TABLE_H
#define HEADER_GUARD_TABLE_H

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <variant>
#include <unordered_map>
#include <ostream>

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
    // Data typed stored in table cell
    using Int32     = uint32_t;
    using Bool      = bool;
    using String    = std::array<signed char,   MAX_STRING_DATA>;
    using Bytes     = std::array<unsigned char, MAX_STRING_DATA>;

    // Flags for of data stored in one table column
    enum ColumnType {
        ColumnTypeInt32,
        ColumnTypeBool,
        ColumnTypeString,
        ColumnTypeBytes
    };

    struct ColumnDescription {
        const ColumnType      type_;
        const std::string     name_;
        const unsigned char   attributes_;

        ColumnDescription(ColumnType type, const char* name, 
            unsigned char attributes);

        ColumnDescription(ColumnType type, const std::string& name, 
            unsigned char attributes);

        ~ColumnDescription() = default;
    };

    typedef
        typename std::variant<Int32, Bool, String, Bytes>
        cell_t;

    // Hash function for indexing String and Bytes types
    struct Hash {
        size_t operator() (const cell_t& cell) const;
    };

    typedef
        typename std::vector<cell_t>
        row_t;

    typedef
        typename std::unordered_map<cell_t, size_t, Hash>
        index_t;

    /* 
        Table of a relational database with fixed name and set of columns
    */
    class Table 
    {
    public:
        // Table is not default constructible,
        // name and columns are required
        Table() = delete;

        Table(std::string&& table_name,
            std::vector<ColumnDescription>&& columns);

        Table(const char* name,
            std::vector<ColumnDescription>&& columns);

        Table(Table&& other) = default;
        Table(const Table& other) = default;

        Table& operator= (Table&& other) = default;
        Table& operator= (const Table& other) = default;

        // Getter for table name
        std::string name();

        // Print the table to the output stream
        void display(std::ostream& os);

        //
        // Modification methods
        //
        void insert_row(row_t&& row);   /*  Insert new row
                                            Replace existing row if one of unique keys already exist */

        void insert_row_unordered(
            std::unordered_map<std::string, cell_t>&& row); /*  Insert new row, represented as a map where
                                                                keys are column names, values are cell data.
                                                                Replace existing row if one of unique keys already exist */
        
        row_t get_row(const std::string& index_column,
            const cell_t& index);   /*  Get row by index
                                        Return empty if index doesn't exist
                                        Throw exception if column doesn't exist or if it's not unique */

        void delete_row(const std::string& index_column,
            const cell_t& index);   /*  Delete row by index
                                        Do nothing if index doesn't exist
                                        Throw exception if column doesn't exist or if it's not unique */

    private:
        const std::string
            name_;          // Table name

        const std::vector<ColumnDescription>
            columns_;       // List of column descriptions (type, name and attributes)

        std::vector<row_t> 
            rows_;          // List of rows

        index_t
            index_;         // Map that associates cell data with row number
    };
} // namespace memdb

#endif // HEADER_GUARD_TABLE_H