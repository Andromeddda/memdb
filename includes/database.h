#ifndef  HEADER_GUARD_DATABASE_H
#define  HEADER_GUARD_DATABASE_H

#include <unordered_map>
#include <memory>


#include "table.h"
#include "db_exceptions.h"

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

#endif // HEADER_GUARD_DATABASE_H
