#include "command/command.hpp"
#include "database/database.hpp"
#include <utility>

namespace memdb 
{

    Result Command::execute(Database* database)
    {
        try
        {
            return root_->execute(database);
        }
        catch (std::exception& ex)
        {
            return Result(ex.what());
        }
    }

    Command::Command(CommandNodePointer root)
    : root_(root)
    { }


    //
    // GetTable
    //
    
    GetTable::GetTable(const std::string& name) :
        name_(name)
    { }

    GetTable::GetTable(const char* name) :
        name_(name)
    { }

    Result GetTable::execute(Database* database)
    {
        Table* table;

        try {
            table = database->get_table(name_);
            return Result(table);
        }
        catch (DatabaseException& ex)
        {
            return Result(ex.what());
        }
    }

    //
    // Create Table
    //

    SQLCreateTable::SQLCreateTable(const std::string& name, const std::vector<Column>& columns) :
        name_(name), columns_(columns)
    { }

    SQLCreateTable::SQLCreateTable(const char*  name, const std::vector<Column>& columns) :
        name_(name), columns_(columns)
    { }

    Result SQLCreateTable::execute(Database* database)
    {
        try
        {
            Table* table = new Table(name_, columns_);
            database->add_table(table);
            return Result(database->get_table(name_));
        }
        catch (DatabaseException& ex)
        {
            return Result(ex.what());
        }
    }

    //
    // Insert
    //

    SQLInsertOrdered::SQLInsertOrdered(const std::string& name, const std::vector<Cell>& data) :
        name_(name), data_(data)
    { }

    SQLInsertOrdered::SQLInsertOrdered(const char*   name, const std::vector<Cell>& data) :
        name_(name), data_(data)
    { }

    Result SQLInsertOrdered::execute(Database* database)
    {
        try
        {
            Table* table = database->get_table(name_);
            table->insert(data_);
            return Result(table);
        }
        catch (DatabaseException& ex)
        {
            return Result(ex.what());
        }
    }


    SQLInsertUnordered::SQLInsertUnordered(const std::string& name, const std::unordered_map<std::string, Cell>& data) :
        name_(name), data_(data)
    { }

    SQLInsertUnordered::SQLInsertUnordered(const char*   name, const std::unordered_map<std::string, Cell>& data) :
        name_(name), data_(data)
    { }


    Result SQLInsertUnordered::execute(Database* database)
    {
        try
        {
            Table* table = database->get_table(name_);
            table->insert(data_);
            return Result(table);
        }
        catch (DatabaseException& ex)
        {
            return Result(ex.what());
        }
    }


    SQLSelect::SQLSelect(const std::vector<std::string>& column_names, 
        CommandNodePointer& argument, Expression& where)
    : column_names_(column_names), argument_(argument), where_(where)
    { }

        // Allocate new table
    Result SQLSelect::execute(Database* database)
    {
        Result arg = argument_->execute(database);
        if (!arg.ok())
            return arg;
        try
        {
            Table* table = arg.get_table();
            Table* ret = table->select(column_names_, where_);
            return Result(ret);
        }
        catch (DatabaseException& ex)
        {
            return Result(ex.what());
        }
    }


    SQLUpdate::SQLUpdate(const std::string& name, 
        std::unordered_map<std::string, Expression>& set, 
        Expression& where)
    : name_(name), set_(std::move(set)), where_(std::move(where))
    { }

    Result SQLUpdate::execute(Database* database)
    {
        try
        {
            Table* table = database->get_table(name_);
            table->update(set_, where_);
            return Result(table);
        }
        catch (DatabaseException& ex)
        {
            return Result(ex.what());
        }
    }


    SQLDelete::SQLDelete(const std::string& name, Expression& where)
    : name_(name), where_(where)
    { }

    Result SQLDelete::execute(Database* database)
    {
        (void)database;
        return Result("TODO: delete");
    }


} // namespace memdb
