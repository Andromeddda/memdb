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
    typedef typename std::unordered_map<std::string, size_t>
        ColumnMap;
    /* 
        Table of a relational database with fixed name and set of columns
    */

    class Expression;

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
            const std::vector<Column>& columns);

        Table(const char* name, 
            const std::vector<Column>& columns);

        // Getter for table name
        std::string name();

        // Print the table to the output stream
        void display(std::ostream& os);

        //
        // Access methods
        //
        size_t column_index(const std::string& column_name);
        size_t width() const;

        //
        // Query methods
        //

        void insert_row_ordered(
            const std::vector<Cell>& data); /*  Insert new row
                                                Replace existing row if one of unique keys already exist */
        void insert_row_ordered(
            std::vector<Cell>&& data);  

        void insert_row_unordered(
            const std::unordered_map<std::string, Cell>& data); /*  Insert new row, represented as a map where
                                                                    keys are column names, values are cell data.
                                                                    Replace existing row if one of unique keys already exist */

        std::shared_ptr<Table> select(
            const std::vector<std::string>& columns, Expression* where);

    private:
        const std::string
            name_;          // Table name

        ColumnMap
            column_map_;       // map of columns

        std::vector<Column>
            columns_;

        std::vector<Row> 
            rows_;          // List of rows
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_TABLE_H