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



#endif // HEADER_GUARD_DB_EXCEPTIONS_H