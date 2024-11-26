#include "database/database.hpp"
#include "parser/parse_exception.hpp"

namespace memdb
{

    Result Database::execute(const std::string& query)
    {
        Parser p(query);
        Command c;

        try 
        {
            p.parse(c);
        }
        catch (ParseException& ex)
        {
            return Result(ex.what());
        }

        // this must be safe, try catch is inside
        return c.execute(this);
    }

    Result Database::execute(const char* query)
    {
        return execute(std::string(query));
    }

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