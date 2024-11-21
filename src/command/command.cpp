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

    // //
    // // Create Table
    // //

    // SQLCreateTable::SQLCreateTable(const std::string& name, ColumnMap& columns) :
    //     name_(name), columns_(columns)
    // { }

    // SQLCreateTable::SQLCreateTable(const char*  name, ColumnMap& columns) :
    //     name_(name), columns_(columns)
    // { }

    // TablePointer SQLCreateTable::execute(Database* database)
    // {
    //     TablePointer table = TablePointer(new Table(name_, columns_));
    //     database->add_table(table);

    //     return table;
    // }

    // //
    // // Insert
    // //

    // SQLInsertOrdered::SQLInsertOrdered(const std::string& name, const std::vector<Cell>& data) :
    //     name_(name), data_(data)
    // { }

    // SQLInsertOrdered::SQLInsertOrdered(const char*   name, const std::vector<Cell>& data) :
    //     name_(name), data_(data)
    // { }


    // SQLInsertUnordered::SQLInsertUnordered(const std::string& name, const std::unordered_map<std::string, Cell>& data) :
    //     name_(name), data_(data)
    // { }

    // SQLInsertUnordered::SQLInsertUnordered(const char*   name, const std::unordered_map<std::string, Cell>& data) :
    //     name_(name), data_(data)
    // { }


    // TablePointer SQLInsert::execute(Database* database)
    // {
    //     TablePointer table = database->get_table(name_);

    //     for (auto [desc, i]& : columns_)
    //     {

    //     }
    // }


} // namespace memdb
