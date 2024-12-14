#ifndef HEADER_GUARD_COMMAND_COMMAND_H
#define HEADER_GUARD_COMMAND_COMMAND_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "command/result.hpp"
#include "database/table.hpp"
#include "expression/expression.hpp"

namespace memdb 
{
    class Database; // forward declaration

    // Abstract class for command tree node
    class SQLCommand 
    {
    public:
        SQLCommand() {}
        virtual ~SQLCommand() {}
        virtual Result execute(Database* database) = 0;
    };

    // Wrapper class for command tree
    class Command
    {
    public:
        Command() = default;     
        Command(const Command& other)             = default;
        Command& operator= (const Command& other) = default;

        Command(Command&& other)             = default;
        Command& operator= (Command&& other) = default;

        Result execute(Database* database);
    private:
        // Only parser can construct command trees
        friend class Parser;
        Command(CommandNodePointer root);

        CommandNodePointer root_;
    };

    // Leave of command tree
    class GetTable : public SQLCommand
    {
    public:
        GetTable(const std::string& name);
        GetTable(const char* name);

        // Return a pointer to existing table from database
        Result execute(Database* database) override;

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
        Result execute(Database* database) override;

    private:
        const std::string name_;    // Name of the table to create
        const std::vector<Column> columns_;
    };

    class SQLInsertOrdered : public SQLCommand
    {
    public:
        SQLInsertOrdered(const std::string& name, const std::vector<Cell>& data);
        SQLInsertOrdered(const char*   name, const std::vector<Cell>& data);

        // Insert a row_ to a table with provided name
        Result execute(Database* database) override;

    private:
        const std::string   name_; // Name of the table to insert to
        std::vector<Cell>   data_;
    };

    class SQLInsertUnordered : public SQLCommand
    {
    public:
        SQLInsertUnordered(const std::string& name, const std::unordered_map<std::string, Cell>& data);
        SQLInsertUnordered(const char*   name, const std::unordered_map<std::string, Cell>& data);

        // Insert a row to a table with provided name
        Result execute(Database* database) override;

    private:
        const std::string   name_; // Name of the table to insert to
        std::unordered_map<std::string, Cell> data_;
    };


    class SQLSelect : public SQLCommand
    {
    public:
        SQLSelect(const std::vector<std::string>& column_names, CommandNodePointer& argument, 
            Expression& where);

        // Allocate new table
        Result execute(Database* database) override;

    private:
        std::vector<std::string> column_names_;  // Pairs of table-column names

        CommandNodePointer
                argument_;   // A table to select from, represented as a Result.
                             // If the table is provided straightforward by name, GetTable class is used
        
        Expression where_;     // Expression tree of conditions provided with WHERE 
    };

    class SQLUpdate : public SQLCommand
    {
    public:
        SQLUpdate(const std::string& name, 
        std::unordered_map<std::string, Expression>& set, 
        Expression& where);

        Result execute(Database* database) override;

    private:
        std::string name_;

        std::unordered_map<std::string, Expression>
            set_;   // Map with keys of column names and values of expressions to assign

        Expression where_;     // Expression tree of conditions provided with WHERE 
    };

    class SQLDelete : public SQLCommand
    {
    public:
        SQLDelete(const std::string& name, Expression& where);

        Result execute(Database* database) override;

    private:
        const std::string name_; // table name
        Expression where_;     // Expression tree of conditions provided with WHERE 
    };

    class SQLJoin;
    class SQLCreateIndex;

} // namespace memdb

#endif // HEADER_GUARD_COMMAND_COMMAND_H