#ifndef HEADER_GUARD_DATABASE_DATABASE_H
#define HEADER_GUARD_DATABASE_DATABASE_H

#include "database/table.hpp"
#include "command/command.hpp"

namespace memdb
{
    class Database
    {
    public:
        Database() = default;

        void 
        add_table(Table* table);

        Table*
        get_table(const std::string& table_name);

        void
        drop_table(const std::string& table_name);

        // TODO: add 'Result execute(string)' method
    private:
        std::unordered_map<std::string, std::shared_ptr<Table>>
            tables_;
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_DATABASE_H