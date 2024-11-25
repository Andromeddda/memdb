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

    //
    // Lexems
    //

    // variable names
    class InvalidNameException: public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Incorrect variable name provided. Name cannot be empty, an operator token, or contain space symbols\n"; 
        }
    };

    // table names
    class InvalidTableNameException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid table name provided\n"; 
        }
    };

    // commands
    class UnknowCommandException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : unknown command\n"; 
        }
    };

    // keywords
    class IncorrectKeywordException  : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : unexpected or incorrect keyword\n"; 
        }
    };

    //
    // Sequences
    //

    // escape secuence in string
    class InvalidEscapeSequenceException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid escape sequence in string\n"; 
        }
    };

    // hex 
    class InvalidHexValueException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid hex value\n"; 
        }
    };

    // column description
    class InvalidColumnDescriptionException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid column description\n"; 
        }
    };

    // set assignment
    class InvalidSetAssignmentException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid list of set assignment\n"; 
        }
    };

    class InvalidRowDataException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid row initializer list\n"; 
        }
    };

    //
    // Lists
    //
    class AttributeException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid attribute list\n"; 
        }
    };

    class InvalidPositionedRowException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : Invalid row initialization by ordered list\n"; 
        }
    };

    class InvalidAssignmentRowException : public ParseException
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

    class InvalidColumnNameListException : public ParseException
    {
    public:
        const char* what() const throw() {
            return "[PARSE ERROR] : invalid column name list\n"; 
        }
    };


} // namespace memdb

#endif // HEADER_GUARD_PARSER_PARSE_EXCEPTION_H