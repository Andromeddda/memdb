#include "parser.h"
#include "command.h"
#include "table.h"
#include "database.h"
#include "where.h"

#include <regex>
#include <string>
#include <cstring>
#include <iostream>
#include <unordered_map>

#include <ctype.h>
#include <assert.h>

namespace memdb 
{
    static const std::unordered_map<std::string, CommandType>
        str_to_command_mp {
            {"CREATE TABLE", CreateTable},
            {"INSERT",     Insert},
            {"UPDATE",     Update},
            {"SELECT",     Select},
            {"DELETE",     Delete},
            {"JOIN",     Join},
            {"CREATE INDEX", CreateIndex}
        };

    static const std::unordered_map<std::string, KeywordType>
        str_to_keyword_mp {
            {"TO",         To},
            {"FROM",     From},
            {"WHERE",     Where},
            {"SET",     Set},
            {"ON",         On},
            {"INDEX ON", IndexOn},
            {"BY",         By}
        };

    // convert escape sequences to characters
    std::string unescape(const std::string& str)
    {
        std::string res = "";
        for (auto i = 0LU; i < str.size(); i++)
        {
            if (str[i] != '\\') {
                res += str[i]; // append normal characters
                continue;
            }

            if (i + 1 == str.size())
                throw InvalidEscapeSequenceException();

            switch(str[++i]) 
            {
            case 'a': res += '\a'; break;
            case 'b': res += '\b'; break;
            case 'f': res += '\f'; break;
            case 'n': res += '\n'; break;
            case 'r': res += '\r'; break;
            case 't': res += '\t'; break;
            case 'v': res += '\v'; break;
            case '\'': res += '\''; break;
            case '\"': res += '\"'; break;
            case '\\': res += '\\'; break;
            case '\?': res += '\?'; break;
            case 'x':
                char hex = 0;
                if (i + 1 == str.size()) throw InvalidEscapeSequenceException();
                if (isxdigit(str[++i])) 
                    hex += str[i];

                if (i + 1 == str.size()) throw InvalidEscapeSequenceException();
                if (isxdigit(str[++i])) { 
                    hex *= 16; 
                    hex += str[i]; 
                }
                res += hex;
                break;  
            }
        }
        return res;
    }

    CommandType str_to_command(std::string& str)
    {
        for (auto i = 0LU; i < str.size(); ++i)
            if (isalpha(str[i]))
                str[i] = toupper(str[i]); // convert all letters to upppercase

        assert(str_to_command_mp.find(str) != str_to_command_mp.end());
        return str_to_command_mp.at(str); // return command type from map
    }

    KeywordType str_to_keyword(std::string& str)
    {
        for (auto i = 0LU; i < str.size(); ++i)
            if (isalpha(str[i]))
                str[i] = toupper(str[i]); // convert all letters to upppercase

        assert(str_to_keyword_mp.find(str) != str_to_keyword_mp.end());
        return str_to_keyword_mp.at(str); // return command type from map
    }

    bool Parser::parse_pattern(std::regex regexp)
    {
        std::smatch match_result{}; // Match regex in std::string

        bool match_status = std::regex_search(
            pos_, end_,                              // The iterators limiting the matched sequence
            match_result,                            // Match result
            regexp,                                  // Regular expression specifying the pattern
            std::regex_constants::match_continuous); // Start matching from the beginning (pos_)

        // Move the iterator on success:
        if (match_status) {
            pos_ = match_result[0].second;
        }

        return match_status;
    }

    bool Parser::parse_pattern(std::regex regexp, std::string& ret) 
    {
        std::smatch match_result{}; // Match regex in std::string

        bool match_status = std::regex_search(
            pos_, end_,                              // The iterators limiting the matched sequence
            match_result,                            // Match result
            regexp,                                  // Regular expression specifying the pattern
            std::regex_constants::match_continuous); // Start matching from the beginning (pos_)

        // Move the iterator on success:
        if (match_status) {
            pos_ = match_result[0].second;
            ret = std::string(match_result[0].first, match_result[0].second);
        }

        return match_status;
    }


    bool Parser::parse_whitespaces()
    {
        static const std::regex pattern{"\\s+"};
        return parse_pattern(pattern);
    }

    bool Parser::parse_comma()
    {
        parse_whitespaces(); // skip whitespaces before comma

        static const std::regex pattern{","};
        bool res = parse_pattern(pattern); // parse comma

        parse_whitespaces(); // skip whitespaces after comma
        return res;
    }

    bool Parser::parse_colon()
    {
        parse_whitespaces(); // skip whitespaces before comma

        static const std::regex pattern{":"};
        bool res = parse_pattern(pattern); // parse comma

        parse_whitespaces(); // skip whitespaces after comma
        return res;
    }

    bool Parser::parse_equal_sign()
    {
        parse_whitespaces(); // skip whitespaces before comma

        static const std::regex pattern{"="};
        bool res = parse_pattern(pattern); // parse comma

        parse_whitespaces(); // skip whitespaces after comma
        return res;
    }

    bool Parser::parse_command(CommandType& ret)
    {
        static const std::regex 
            pattern{"(?i)(CREATE\\s+TABLE)|(INSERT)|(SELECT)|(UPDATE)|(DELETE)"};
        // Note: (?i) is a flag that makes pattern  case insensitive

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = str_to_command(str);     // assign ret to matched pattern
        return res;
    }

    bool Parser::parse_keyword(KeywordType& ret)
    {
        static const std::regex 
            pattern{"(?i)(TO)|(FROM)|(WHERE)|(SET)|(ON)|(INDEX\\s+ON)|(BY)"};
        // Note: (?i) is a flag that makes pattern  case insensitive

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = str_to_keyword(str); // assign ret to matched pattern
        return res;
    }

    bool Parser::parse_name(std::string& ret)
    {
        static const std::regex 
            pattern{"[A-Za-z0-9_\\-]+"};
        return parse_pattern(pattern, ret);
    }

    bool Parser::parse_subquery(std::string& ret) 
    {
        static const std::regex 
            pattern{"\\(.+\\)"};
        return parse_pattern(pattern, ret);
    }

    bool Parser::parse_int(int& ret)
    {
        static const std::regex
            pattern("[1-9][0-9]*");

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = std::stoi(str);

        return res;
    }

    bool Parser::parse_string(std::string& ret)
    {
        static const std::regex
            pattern(".+");

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = str;

        return res;
    }

    // bool Parser::parse_row_ordered(row_t& ret)
    // {
    //     static const std::regex 
    //         pattern{"\\(.+\\)"};
    //     return parse_pattern(pattern, ret);
    // }

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