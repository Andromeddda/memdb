#ifndef HEADER_GUARD_COMMAND_H
#define HEADER_GUARD_COMMAND_H

#include <memory>

#include "table.h"
#include "database.h"
#include "expression.h"

namespace memdb 
{
    class SQLCommand 
    {
    public:
        SQLCommand();
        virtual ~SQLCommand();
        virtual TablePointer execute(Database* database) = 0;
    };

    class GetTable;
    class SQLCreateTable;
    class SQLInsert;
    class SQLSelect;
    class SQLUpdate;
    class SQLJoin;
    class SQLCreateIndex;

} // namespace memdb

#endif // HEADER_GUARD_COMMAND_H