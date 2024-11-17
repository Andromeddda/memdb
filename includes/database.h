#ifndef  HEADER_GUARD_DATABASE_H
#define  HEADER_GUARD_DATABASE_H

#include <unordered_map>

#include "table.h"
#include "db_exceptions.h"

namespace memdb 
{
    class Database
    {
    public:
        Database() = default;

        void    add_table(Table* table);
        Table*  get_table(const std::string& table_name);
        void    drop_table(const std::string& table_name);
    private:
        std::unordered_map<std::string, Table*>
            tables_;
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_H
