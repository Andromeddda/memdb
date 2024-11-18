#ifndef HEADER_GUARD_PARSER_H
#define HEADER_GUARD_PARSER_H

#include "where.h"
#include "assignment.h"
#include "command.h"
#include "table.h"

namespace memdb 
{
    enum CommandType
    {
        CreateTable,
        Insert,
        Update,
        Select,
        Delete,
        Join,
        CreateIndex
    };

    enum KeywordType 
    {
        To,
        From,
        Where,
        Set,
        On,
        IndexOn,
        By
    };

	class Parser
    {
    public:
        Parser(std::string&& query);

        // Parser is neither copyable nor default constructible
        Parser() = delete;
        Parser(const Parser& other) = delete;
        Parser& operator= (const Parser& other) = delete;

        // Move semantics
        Parser(Parser&& other) = default;
        Parser& operator= (Parser&& other) = default;

        SQLCommand* parse();

    private:
        typedef std::string::iterator Position;
        std::string query_;

        bool parse_pattern(std::regex regexp);
        bool parse_pattern(std::regex regexp, std::string& ret);

        bool parse_whitespaces();
        bool parse_comma();

        bool parse_command_name(CommandType& ret);
        bool parse_keyword(KeywordType& ret);
        bool parse_table_name(std::string& ret);
        bool parse_table_or_subquery(std::string& ret);

        bool parse_row_ordered(row_t& ret);
        bool parse_row_unordered(std::unordered_map<std::string, cell_t>& ret);
        bool parse_column_name(std::pair<std::string, std::string>& ret);
        bool parse_column_description(ColumnDescription& ret);
        bool parse_where_condition(WhereStatement& ret);
        bool parse_set_assignment(SetAssignment& ret);

        Position pos_;
        Position end_;

    };
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

#endif // HEADER_GUARD_PARSER_H