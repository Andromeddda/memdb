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
    /* 
        Table of a relational database with fixed name and set of columns
    */

    class Expression;

    class Table 
    {
    public:
        // Tables are not default constructible or copyable
        Table()                                 = default;
        Table(const Table& other)               = delete;
        Table& operator= (const Table& other)   = delete;

        // Tables are movable
        Table(Table&& other)                    = default;
        Table& operator= (Table&& other)        = default;

        // Construct with name and map of columns
        Table(const std::string& table_name, 
            const std::vector<Column>& columns);

        Table(const char* name, 
            const std::vector<Column>& columns);

        
        std::string name();     // Table name
        size_t width() const;   // Number of columns
        size_t size() const;    // Number of rows
        size_t column_position(const std::string& column_name) const;

        //
        // Query methods
        //

        void insert(const std::vector<Cell>& data);
        void insert(std::vector<Cell>&& data);  
        void insert(const std::unordered_map<std::string, Cell>& data);

        void update(const std::unordered_map<std::string, Expression>& assignment, 
            const Expression& where);

        Table* select(const std::vector<std::string>& columns, const Expression& where);

        void drop(const Expression& where);

        void print(std::ostream& os);

    private:
        std::string
            name_;          // Table name

        std::unordered_map<std::string, size_t>
            column_positions_;    // map of columns

        std::vector<Column>
            columns_;

        std::unordered_map<size_t, std::shared_ptr<Row>> 
            rows_;          // List of rows
    };
} // namespace memdb


#endif // HEADER_GUARD_DATABASE_TABLE_H