#include "parser.h"
#include "command.h"
#include "table.h"
#include "database.h"
#include "where.h"

#include <regex>
#include <string>
#include <cstring>
#include <iostream>
#include <ctype.h>

namespace memdb 
{

} // namespace memdb



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