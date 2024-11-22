#ifndef HEADER_GUARD_COMMAND_COMMAND_H
#define HEADER_GUARD_COMMAND_COMMAND_H

#include "database/database.hpp"
#include "expression/expression.hpp"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace memdb 
{
    // Note: TablePointer = std::shared_ptr<Table>

    class SQLCommand 
    {
    public:
        SQLCommand() {}
        virtual ~SQLCommand() {}
        virtual TablePointer execute(Database* database) = 0;
    };

    typedef std::unique_ptr<Expression> ExpressionPointer;
    typedef std::unique_ptr<SQLCommand> CommandPointer;

    class GetTable : public SQLCommand
    {
    public:
        GetTable(const std::string& name);
        GetTable(const char* name);

        // Return a pointer to existing table from database
        TablePointer execute(Database* database) override;

    private:
        const std::string name_;
    };

    class SQLCreateTable : public SQLCommand
    {
    public:
        SQLCreateTable(const std::string& name, const std::vector<Column>& columns);
        SQLCreateTable(const char*  name, const std::vector<Column>& columns);

        // Allocate table
        // Return pointer to it
        TablePointer execute(Database* database) override;

    private:
        const std::string name_; // Name of the table to create

        const std::vector<Column> columns_; // Column descriptions
    };

    class SQLInsertOrdered : public SQLCommand
    {
    public:
        SQLInsertOrdered(const std::string& name, const std::vector<Cell>& data);
        SQLInsertOrdered(const char*   name, const std::vector<Cell>& data);

        // Insert a row_ to a table with provided name
        // Return a smart pointer to the same table
        TablePointer execute(Database* database) override;

    private:
        const std::string   name_; // Name of the table to insert to
        std::vector<Cell>   data_;
    };

    class SQLInsertUnordered : public SQLCommand
    {
    public:
        SQLInsertUnordered(const std::string& name, const std::unordered_map<std::string, Cell>& data);
        SQLInsertUnordered(const char*   name, const std::unordered_map<std::string, Cell>& data);

        // Insert a row_ to a table with provided name
        // Return a smart pointer to the same table
        TablePointer execute(Database* database) override;

    private:
        const std::string   name_; // Name of the table to insert to
        std::unordered_map<std::string, Cell> data_;
    };


    class SQLSelect : public SQLCommand
    {
    public:
        SQLSelect(const std::vector<std::string>& column_names, CommandPointer& argument, 
            ExpressionPointer& where);

        // Allocate new table
        TablePointer execute(Database* database) override;

    private:
        std::vector<std::string> column_names_;  // Pairs of table-column names

        CommandPointer argument_;   // A table to select from, represented as a command.
                                    // If the table is provided straightforward by name, GetTable class is used
        
        ExpressionPointer where_;     // Expression tree of conditions provided with WHERE 
    };

    class SQLUpdate : public SQLCommand
    {
    public:
        SQLUpdate(const std::vector<std::string>& column_names, 
            std::unordered_map<std::string, ExpressionPointer>& set, 
            ExpressionPointer& where);

        TablePointer execute(Database* database) override;

    private:
        std::vector<std::string> column_names_;  // Pairs of table-column names

        std::unordered_map<std::string, ExpressionPointer>
            set_;   // Map with keys of column names and values of expressions to assign

        ExpressionPointer where_;     // Expression tree of conditions provided with WHERE 
    };

    class SQLDelete : public SQLCommand
    {
    public:
        SQLDelete(const ExpressionPointer& where);

        TablePointer execute(Database* database) override;

    private:
        ExpressionPointer where_;     // Expression tree of conditions provided with WHERE 
    };

    class SQLJoin;
    class SQLCreateIndex;

} // namespace memdb

#endif // HEADER_GUARD_COMMAND_COMMAND_H