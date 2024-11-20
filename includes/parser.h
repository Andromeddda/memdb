#ifndef HEADER_GUARD_PARSER_H
#define HEADER_GUARD_PARSER_H

#include <regex>
#include <string>
#include <cstddef>

#include "expression.h"
#include "command.h"

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
        typedef std::string::const_iterator Position;
        std::string query_;

        // general parsing functions
        bool parse_pattern(std::regex regexp);
        bool parse_pattern(std::regex regexp, std::string& ret);

        // punctuation parsing
        bool parse_whitespaces();
        bool parse_comma();
        bool parse_colon();
        bool parse_equal_sign();

        // parsing lexems
        bool parse_command(CommandType& ret);
        bool parse_keyword(KeywordType& ret);
        bool parse_name(std::string& ret);
        bool parse_subquery(std::string& ret);

        // parsing values
        bool parse_int(int& ret);
        bool parse_string(std::string& ret);
        bool parse_bool(bool& ret);
        bool parse_bytes(std::vector<std::byte>& ret);
        bool parse_cell_data(cell_t& ret);

        // parsing column description
        bool parse_attribute(ColumnAttribute& ret);
        bool parse_attribute_list(unsigned char& ret);
        bool parse_column_type(ColumnType& ret);
        bool parse_column_description(ColumnDescription &ret);
        bool parse_column_description_list(columns_t& ret);

        // parsing rows
        bool parse_row_ordered(row_t& ret);
        bool parse_row_unordered(std::unordered_map<std::string, cell_t>& ret);

        bool parse_column_name(std::pair<std::string, std::string>& ret);
        bool parse_expression(Expression& ret);

        using VecPosition = typename std::vector<std::string>::const_iterator;

        static bool parse_pattern_static(std::regex regexp, Position& pos_, Position& end_);
        static bool parse_pattern_static(std::regex regexp, Position& pos_, Position& end_, std::string& ret);
        static std::vector<std::string> tokenize_expression(const std::string& str);
        static std::unique_ptr<Expression> parse_expression(const std::vector<std::string>& tokens);
        static std::unique_ptr<Expression> parse_expression_r(const std::vector<std::string>& tokens, 
            VecPosition pos, VecPosition end);

        Position pos_;
        Position end_;

    };
} // namespace memdb


/* 

Commands:

1.
+ CREATE TABLE <NAME> [{attributes}] <name>: <type> [= <value>]
        create table users ({key, autoincrement} id :
        int32, {unique} login: string[32], password_hash: bytes[8], is_admin:
        bool = false)

+ INSERT (<values>) TO <table>
        insert (,"vasya", 0xdeadbeefdeadbeef) to users
        insert (login = "vasya", password_hash = 0xdeadbeefdeadbeef)

- SELECT <columns> FROM <table> [WHERE <condition>]
        select id, login from users where is_admin || id < 10

- UPDATE <table> SET <assignments> [WHERE <condition>]
- DELETE <table> WHERE <condition>

2. 
- <table1> JOIN <table2> ON <condition>
- CREATE <index type> INDEX ON <table> BY <columns>

*/

#endif // HEADER_GUARD_PARSER_H