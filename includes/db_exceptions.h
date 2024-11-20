#ifndef HEADER_GUARD_DB_EXCEPTIONS_H
#define HEADER_GUARD_DB_EXCEPTIONS_H

#include <exception>

class TableAlreadyExistException : public std::exception
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

class TableDoNotExistException : public std::exception
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

class InvalidTablePointerException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid table pointer provided\n"; 
	}
};

class InvalidEscapeSequenceException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid escape sequence in string\n"; 
	}
};

class InvalidHexValueException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid hex value\n"; 
	}
};

class MaxLengthExceededException : public std::exception
{
public:
	const char* what() const throw() {
		return "String or bytes data exceeded max length of 256\n"; 
	}
};

class TypeException : public std::exception
{
public:
	const char* what() const throw() {
		return "Unable to convert cell typed to requested type\n"; 
	}
};

class AttributeException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid attribute list\n"; 
	}
};

class InvalidColumnDescriptionException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid column description\n"; 
	}
};

class InvalidColumnNameException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid column name\n"; 
	}
};

class InvalidAssignmentRowException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid row initialization by unordered assignment\n"; 
	}
};

class InvalidPositionedRowException : public std::exception
{
public:
	const char* what() const throw() {
		return "Invalid row initialization by unordered assignment\n"; 
	}
};

class IncompatibleTypeOperatorException: public std::exception
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


class DifferentTypesException: public std::exception
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

class DifferentSizeException: public std::exception
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


class DivisionByZeroException: public std::exception
{
public:
	const char* what() const throw() {
		return "Division by zero\n"; 
	}
};





#endif // HEADER_GUARD_DB_EXCEPTIONS_H