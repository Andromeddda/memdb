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
            return "Invalid escape sequence in string\n"; 
        }
    };

    class InvalidHexValueException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "Invalid hex value\n"; 
        }
    };

    class AttributeException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "Invalid attribute list\n"; 
        }
    };

    class InvalidColumnDescriptionException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "Invalid column description\n"; 
        }
    };

    class InvalidColumnNameException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "Invalid column name\n"; 
        }
    };

    class InvalidAssignmentRowException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "Invalid row initialization by unordered assignment\n"; 
        }
    };

    class InvalidPositionedRowException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "Invalid row initialization by unordered assignment\n"; 
        }
    };

    class IncorrectParenthesisException: public ParseException
    {
    public:
        const char* what() const throw() {
            return "Incorrect sequence of parenthesis\n"; 
        }
    };

    class EmptyExpressionException: public ParseException
    {
    public:
        const char* what() const throw() {
            return "Empty expression or empty parenthesis\n"; 
        }
    };

    class IncorrectNameException: public ParseException
    {
    public:
        const char* what() const throw() {
            return "Incorrect variable name provided to exception. Name cannot be empty, an operator token, or contain space symbols\n"; 
        }
    };

} // namespace memdb

#endif // HEADER_GUARD_PARSER_PARSE_EXCEPTION_H