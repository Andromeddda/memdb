#ifndef HEADER_GUARD_COMMAND_H
#define HEADER_GUARD_COMMAND_H

#include "table.h"
#include "database.h"

/* 

Commands:

1. 
- CREATE TABLE <NAME> [{attributes}] <name>: <type> [= <value>]
- INSERT (<values>) TO <table>
- SELECT <columns> FROM <table> [WHERE <condition>]
- UPDATE <table> SET <assignments> [WHERE <condition>]
- DELETE <table> WHERE <condition>

2. 
- <table1> JOIN <table2> ON <condition>
- CREATE <index type> INDEX ON <table> BY <columns>

*/

namespace memdb 
{
    class SQLCommand 
    {
    public:
        SQLCommand();
        virtual ~SQLCommand();
        virtual Table* execute(Database* database) = 0;
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