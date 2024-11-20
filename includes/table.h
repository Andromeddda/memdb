#ifndef    HEADER_GUARD_TABLE_H
#define HEADER_GUARD_TABLE_H

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <variant>
#include <unordered_map>
#include <map>
#include <ostream>
#include <cstddef>


#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <cstdint>

#include "cell.h"

#define MAX_TABLE_NAME  128U
#define MAX_COLUMN_NAME 128U

namespace memdb 
{
    enum ColumnAttribute
    {
        Unique = 1,
        Key = 2,
        Autoincrement = 4
    };

    struct ColumnDescription 
    {
        ColumnType      type_;
        std::string     name_;
        unsigned char   attributes_;

        ColumnDescription();
        ColumnDescription(const char *name);
        ColumnDescription(const std::string& name);
        ColumnDescription(ColumnType type, const char* name, 
            unsigned char attributes);

        ColumnDescription(ColumnType type, const std::string& name, 
            unsigned char attributes);

        ColumnDescription(const ColumnDescription& other) = default;
        ColumnDescription(ColumnDescription&& other) = default;

        ColumnDescription& operator=(const ColumnDescription& other) = default;
        ColumnDescription& operator=(ColumnDescription&& other) = default;

        bool operator== (const ColumnDescription& other) const;

        ~ColumnDescription() = default;
    };

    typedef Cell cell_t;

    // Lexicographical comparison of two cells
    struct CellCompare {
        bool operator() (const cell_t& lhs, const cell_t& rhs) const {
            return lhs.less(rhs);
        }
    };

    // Lexicographical comparison of two cells
    struct ColumnHash {
        size_t operator() (const ColumnDescription& lhs) const {
            return std::hash<std::string>{}(lhs.name_);
        }
    };

    typedef
        typename std::vector<cell_t>
        row_t;

    typedef
        typename std::map<cell_t, size_t>
        index_t;

    typedef
        typename std::unordered_map<ColumnDescription, size_t, ColumnHash>
        columns_t;

    /* 
        Table of a relational database with fixed name and set of columns
    */
    class Table 
    {
    public:
        // Table is not default constructible,
        // name and columns are required
        Table() = delete;

        Table(std::string&& table_name, columns_t&& columns);

        Table(const char* name, columns_t&&  columns);

        Table(Table&& other) = default;
        Table(const Table& other) = default;

        Table& operator= (Table&& other) = default;
        Table& operator= (const Table& other) = default;

        // Getter for table name
        std::string name();

        // Print the table to the output stream
        void display(std::ostream& os);

        //
        // Access methods
        //
        size_t column_index(const std::string& column_name);

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

        const columns_t
            columns_;       // map of columns

        std::vector<row_t> 
            rows_;          // List of rows

        index_t
            index_;         // Map that associates cell data with row number
    };
} // namespace memdb

#endif // HEADER_GUARD_TABLE_H