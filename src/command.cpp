#include "command.h"
#include <utility>

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

        // Return a pointer to existing table from database
        TablePointer execute(Database* database) override;

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


        // Allocate table
        // Return pointer to it
        TablePointer execute(Database* database) override;

    private:
        const std::string
            name_; // Name of the table to create

        const std::vector<ColumnDescription>
            columns_; // Column descriptions
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

        // Insert a row_ to a table with provided name
        // Return a smart pointer to the same table
        TablePointer execute(Database* database) override;

    private:
        const std::string
            name_; // Name of the table to insert to

        row_t
            row_; // A row to insert
    };

    class SQLSelect : public SQLCommand
    {
    public:
       	SQLSelect();

        // Allocate new table
        TablePointer execute(Database* database) override;

    private:
   		std::vector<std::pair<std::string, std::string>>
   			column_names_;  // Pairs of table-column names

        SQLCommand*
            argument_;  // A table to select from, represented as a command.
                        // If the table is provided straightforward by name, GetTable class is used

        std::unique_ptr<Expression>
            where_;     // Expression tree of conditions provided with WHERE 
    };

    class SQLUpdate : public SQLCommand
    {
    public:
        SQLUpdate();

        TablePointer execute(Database* database) override;

    private:
        std::vector<std::pair<std::string, std::string>>
            column_names_;  // Pairs of table-column names

        std::unique_ptr<Expression>
            where_;     // Expression tree of conditions provided with WHERE 
    };


} // namespace memdb
