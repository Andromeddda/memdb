#include "database.h"

namespace memdb
{
    void Database::add_table(Table* table) 
    {
        std::string name = table->name();
        
        if (tables_.find(name) != tables_.end())
            throw TableAlreadyExistException(name);

        if (table == nullptr)
            throw InvalidTablePointerException();

        tables_[name] = table;
    }

    Table* Database::get_table(const std::string& name)
    {
        if (tables_.find(name) == tables_.end())
            throw TableDoNotExistException(name);

        return tables_.at(name);
    }

    void Database::drop_table(const std::string& name)
    {
        if (tables_.find(name) == tables_.end())
            throw TableDoNotExistException(name);

        tables_.erase(name);
    }
} // namespace memdb