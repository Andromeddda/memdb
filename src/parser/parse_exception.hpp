#ifndef HEADER_GUARD_PARSER_PARSE_EXCEPTION_H
#define HEADER_GUARD_PARSER_PARSE_EXCEPTION_H

#include <exception>

namespace memdb 
{
    class ParseException : public std::exception
    {
    public:
        ParseException() = default;
    };

    class InvalidEscapeSequenceException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid escape sequence in string\n"; 
        }
    };

    class InvalidHexValueException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid hex value\n"; 
        }
    };

    class AttributeException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid attribute list\n"; 
        }
    };

    class InvalidColumnDescriptionException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid column description\n"; 
        }
    };

    class InvalidTableNameException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid table name\n"; 
        }
    };

    class InvalidAssignmentRowException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid row initialization by unordered assignment\n"; 
        }
    };

    class InvalidSetAssignmentException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid list of set assignment\n"; 
        }
    };

    class InvalidPositionedRowException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid row initialization by unordered assignment\n"; 
        }
    };

    class IncorrectParenthesisException: public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Incorrect sequence of parenthesis\n"; 
        }
    };

    class EmptyExpressionException: public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Empty expression or empty parenthesis\n"; 
        }
    };

    class IncorrectNameException: public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Incorrect variable name provided to exception. Name cannot be empty, an operator token, or contain space symbols\n"; 
        }
    };

    class UnknowCommandException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : unknown command\n"; 
        }
    };

    class InvalidColumnNameListException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : invalid column name list\n"; 
        }
    };

    class NoFromKeywordException  : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : expected FROM keyword\n"; 
        }
    };


} // namespace memdb

#endif // HEADER_GUARD_PARSER_PARSE_EXCEPTION_H