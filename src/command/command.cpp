#include "command/command.hpp"
#include <utility>

namespace memdb 
{
    //
    // GetTable
    //
    
    GetTable::GetTable(const std::string& name) :
        name_(name)
    { }

    GetTable::GetTable(const char* name) :
        name_(name)
    { }

    TablePointer GetTable::execute(Database* database)
    {
        return database->get_table(name_);
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

    TablePointer SQLCreateTable::execute(Database* database)
    {
        TablePointer table = TablePointer(new Table(name_, columns_));
        database->add_table(table);

        return table;
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

    TablePointer SQLInsertOrdered::execute(Database* database)
    {
        TablePointer table = database->get_table(name_);

        table->insert_row_ordered(data_);

        return table;
    }


    SQLInsertUnordered::SQLInsertUnordered(const std::string& name, const std::unordered_map<std::string, Cell>& data) :
        name_(name), data_(data)
    { }

    SQLInsertUnordered::SQLInsertUnordered(const char*   name, const std::unordered_map<std::string, Cell>& data) :
        name_(name), data_(data)
    { }


    TablePointer SQLInsertUnordered::execute(Database* database)
    {
        TablePointer table = database->get_table(name_);

        table->insert_row_unordered(data_);

        return table;
    }


    SQLSelect::SQLSelect(const std::vector<std::string>& column_names, 
        CommandPointer& argument, ExpressionPointer& where)
    : column_names_(column_names), argument_(std::move(argument)), where_(std::move(where))
    { }

        // Allocate new table
    TablePointer SQLSelect::execute(Database* database)
    {
        TablePointer table = argument_->execute(database);

        return table->select(column_names_, where_.get());
    }


    SQLUpdate::SQLUpdate(const std::string& name, 
        std::unordered_map<std::string, ExpressionPointer>& set, 
        ExpressionPointer& where)
    : name_(name), set_(std::move(set)), where_(std::move(where))
    { }

    TablePointer SQLUpdate::execute(Database* database)
    {
        TablePointer table = database->get_table(name_);

        table->update(set_, where_.get());

        return table;
    }


} // namespace memdb
