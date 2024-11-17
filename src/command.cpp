#include "command.h"
#include <utility>

/* 

Commands:

1. 
- CREATE TABLE <NAME> [{attributes}] <name>: <type> [= <value>]
- INSERT (<values>) TO <table>
- SELECT <columns> FROM <table> [WHERE <condition>]
- UPDATE <table> SET <assignments> [WHERE <condition>]
- DELETE <table> WHERE <condition>

2. 
- <table1> JOIN <table2> ON <condition>
- CREATE <index type> INDEX ON <table> BY <columns>

*/

namespace memdb {
    class GetTable : public SQLCommand
    {
    public:
        GetTable(
            std::string&& name)
        : name_(name) {}

        GetTable(
            const char* name)
        : name_(name) {}

        // Returns a table from database by name
        Table* execute(Database* database) override;

    private:
        const std::string
            name_;
    };

    class SQLCreateTable : public SQLCommand
    {
    public:
        SQLCreateTable(
            std::string&& name, 
            std::vector<ColumnDescription>&& columns)
        : name_(name), columns_(columns) {}

        SQLCreateTable(
            const char*   name, 
            std::vector<ColumnDescription>&& columns)
        : name_(name), columns_(columns) {}

        ~SQLCreateTable() = default;


        // Allocates table and returns pointer to it
        Table* execute(Database* database) override;

    private:
        const std::string
            name_;

        const std::vector<ColumnDescription>
            columns_;
    };
        class SQLInsert : public SQLCommand
        {
        public:
            SQLInsert(
                std::string&& name, 
                row_t&& row)
            : name_(name), row_(row) {}

            SQLInsert(
                const char*   name, 
                row_t&& row)
            : name_(name), row_(row) {}

            ~SQLInsert() = default;

            Table* execute(Database* database) override;

        private:
            const std::string
                name_;

            row_t
                row_;
        };

    class SQLSelect : public SQLCommand
    {
    public:
       	SQLSelect();
        Table* execute(Database* database) override;

    private:
   		std::vector<std::pair<std::string, std::string>>
   			column_names_; // Pairs of table-column names

        SQLCommand*
            argument_;


    };


} // namespace memdb
