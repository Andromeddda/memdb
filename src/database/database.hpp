#ifndef HEADER_GUARD_DATABASE_DATABASE_H
#define HEADER_GUARD_DATABASE_DATABASE_H

#include "database/table.hpp"

namespace memdb
{
    typedef std::shared_ptr<Table>
        TablePointer;

    class Database
    {
    public:
        Database() = default;

        void 
        add_table(TablePointer table);

        TablePointer 
        get_table(const std::string& table_name);

        void
        drop_table(const std::string& table_name);
    private:
        std::unordered_map<std::string, TablePointer>
            tables_;
    };
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_DATABASE_H