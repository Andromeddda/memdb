#ifndef HEADER_GUARD_DB_EXCEPTIONS_H
#define HEADER_GUARD_DB_EXCEPTIONS_H

#include <exception>

namespace memdb
{
    class DatabaseException : public std::exception
    {
    public:
        DatabaseException() = default;
    };

    class MaxLengthExceededException : public DatabaseException
    {
    public:
        const char* what() const throw() {
            return "String or bytes data exceeded max length of 256\n"; 
        }
    };

    class TypeException : public DatabaseException
    {
    public:
        const char* what() const throw() {
            return "Unable to convert cell typed to requested type\n"; 
        }
    };

    class TableAlreadyExistException : public DatabaseException
    {
        const std::string table_name_;
    public:
        TableAlreadyExistException(const std::string& table_name)
        : table_name_(table_name) {}

        ~TableAlreadyExistException() = default;

        const char* what() const throw() {
            auto what_ = "Table named \"" + table_name_ + "\" already exist.\n";
            return what_.c_str(); 
        }
    };

    class TableDoNotExistException : public DatabaseException
    {
        const std::string table_name_;
    public:
        TableDoNotExistException(const std::string& table_name)
        : table_name_(table_name) {}

        ~TableDoNotExistException() = default;

        const char* what() const throw() {
            auto what_ = "Table named \"" + table_name_ + "\" do not exist.\n";
            return what_.c_str(); 
        }
    };

    class InvalidTablePointerException : public DatabaseException
    {
    public:
        const char* what() const throw() {
            return "Invalid table pointer provided\n"; 
        }
    };

    class IncompatibleTypeOperatorException: public DatabaseException
    {
    public:
        IncompatibleTypeOperatorException(
            std::string op, std::string type) :
        op_(op), type_(type) {}

        const char* what() const throw() {
            std::string what_ = "Type " + type_ + " is incompatible with operator " + op_ + '\n';
            return what_.c_str(); 
        }

    private:
        std::string op_;
        std::string type_;
    };


    class DifferentTypesException: public DatabaseException
    {
    public:
        DifferentTypesException(
            std::string op) : op_(op ) { }

        const char* what() const throw() {
            std::string what_ = "Operator " + op_ + " do not maintain different types\n";
            return what_.c_str(); 
        }

    private:
        std::string op_;
    };

    class DifferentSizeException: public DatabaseException
    {
    public:
        DifferentSizeException(
            std::string op) : op_(op ) { }

        const char* what() const throw() {
            std::string what_ = "Operator " + op_ + " do not maintain different size of operands\n";
            return what_.c_str(); 
        }

    private:
        std::string op_;
    };


    class DivisionByZeroException: public DatabaseException
    {
    public:
        const char* what() const throw() {
            return "Division by zero\n"; 
        }
    };

    class UnexistingColumnException: public DatabaseException
    {
    public:
        UnexistingColumnException(
            std::string name) : name_(name) { }

        const char* what() const throw() {
            std::string what_ = "Requested column " + name_ + " does not exist\n";
            return what_.c_str(); 
        }
    private:
        std::string name_;
    };

    class InvaludNumberOfOperandsException: public DatabaseException
    {
    public:
        InvaludNumberOfOperandsException(
            std::string op) : op_(op ) { }

        const char* what() const throw() {
            std::string what_ = "Invalud number of operands provided to operator " + op_ + "\n";
            return what_.c_str(); 
        }

    private:
        std::string op_;
    };


    class IncompatibleTableRowException: public DatabaseException
    {
    public:
        const char* what() const throw() {
            return "Recieved row incompatible with table. Cannot insert\n"; 
        }
    };

} // namespace memdb 

#endif // HEADER_GUARD_DB_EXCEPTIONS_H