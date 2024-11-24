#include "database/database.hpp"

namespace memdb
{
    void Database::add_table(Table* table) 
    {
        std::string name = table->name();
        
        if (tables_.find(name) != tables_.end())
            throw TableAlreadyExistException(name);

        tables_[name] = std::shared_ptr<Table>(table);
    }

    Table* Database::get_table(const std::string& name)
    {
        if (tables_.find(name) == tables_.end())
            throw TableDoNotExistException(name);

        return tables_.at(name).get();
    }

    void Database::drop_table(const std::string& name)
    {
        if (tables_.find(name) == tables_.end())
            throw TableDoNotExistException(name);

        tables_.erase(name);
    }
} // namespace memdb