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
    // typedef typename std::unordered_map<std::string, size_t>
    //     std::unordered_map<std::string, size_t>;
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
        Column get_column(const std::string& column_name);
        size_t width() const;
        size_t size() const;

        //
        // Query methods
        //

        void insert_row_ordered(
            const std::vector<Cell>& data);

        void insert_row_ordered(
            std::vector<Cell>&& data);  

        void insert_row_unordered(
            const std::unordered_map<std::string, Cell>& data);

        std::shared_ptr<Table> select(
            const std::vector<std::string>& columns, Expression* where);

        void delete_where(Expression* where);

        void update(
            const std::unordered_map<std::string, std::unique_ptr<Expression>>& assignment, 
            Expression* where);
    private:
        const std::string
            name_;          // Table name

        std::unordered_map<std::string, size_t>
            column_map_;    // map of columns

        std::vector<Column>
            columns_;

        std::unordered_map<size_t, std::unique_ptr<Row>> 
            rows_;          // List of rows
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_TABLE_H