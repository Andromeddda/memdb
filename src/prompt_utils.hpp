#ifndef HEADER_GUARD_PROMPT_UTILS_H
#define HEADER_GUARD_PROMPT_UTILS_H

static const char help[] = 
    "\n=== memdb ===\n\n.quit or .exit - terminate the program\n\n\
.history - show session history\n\n\
CREATE TABLE <name> <column descriptions>\n\t column description: ([{key | unique | autoincrement} <column_name> : <type>])\n\n\
SELECT <column list> FROM <table> [WHERE <condition>]\n\n\
INSERT <row> TO <table>\n\n\
UPDATE <table> SET <assignments>\n\t assignment: <column_name> = <expression>\n\n\
DELETE <table> WHERE <contition>\n\n";

#endif // HEADER_GUARD_PROMPT_UTILS_H
