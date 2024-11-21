#ifndef HEADER_GUARD_DATABASE_TABLE_H
#define HEADER_GUARD_DATABASE_TABLE_H

#include <ostream>
#include <string>
#include <unordered_map>

#include "database/row.hpp"
#include "database/column.hpp"
#include "database/db_exception.hpp"

namespace memdb
{
    typedef typename std::unordered_map<Column, size_t, ColumnHash>
        ColumnMap;
    /* 
        Table of a relational database with fixed name and set of columns
    */
    class Table 
    {
    public:
        // Tables are not default constructible, copyable nor movable
        Table()                                 = delete;
        Table(Table&& other)                    = delete;
        Table(const Table& other)               = delete;
        Table& operator= (Table&& other)        = delete;
        Table& operator= (const Table& other)   = delete;

        // Construct with name and map of columns
        Table(const std::string& table_name, 
            const ColumnMap& columns);

        Table(const char* name, 
            const ColumnMap& columns);

        // Getter for table name
        std::string name();

        // Print the table to the output stream
        void display(std::ostream& os);

        //
        // Access methods
        //
        size_t column_index(const std::string& column_name);

        void insert_row_ordered(
            const std::vector<Cell>& data); /*  Insert new row
                                                Replace existing row if one of unique keys already exist */  

        void insert_row_unordered(
            const std::unordered_map<std::string, Cell>& data); /*  Insert new row, represented as a map where
                                                                    keys are column names, values are cell data.
                                                                    Replace existing row if one of unique keys already exist */
        Row& get_row(const std::string& index_column,
            const Cell& index);   /*    Get row by index
                                        Return empty if index doesn't exist
                                        Throw exception if column doesn't exist or if it's not unique */

        void delete_row(const std::string& index_column,
            const Cell& index);   /*    Delete row by index
                                        Do nothing if index doesn't exist
                                        Throw exception if column doesn't exist or if it's not unique */

        size_t width() const;

    private:
        const std::string
            name_;          // Table name

        const ColumnMap
            columns_;       // map of columns

        std::vector<Row> 
            rows_;          // List of rows
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_TABLE_H