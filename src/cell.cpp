#include "cell.h"

#include <algorithm>

namespace memdb {

	Cell::Cell(Int32 value) : 
		value_(value)
	{ }

	Cell::Cell(Bool value) : 
		value_(value)
	{ }

	Cell::Cell(std::string& value)
	{ 
		if (value.size() > MAX_STRING_DATA)
			throw MaxLengthExceededException();

		String a{0}; // Initialize array with zeros
		std::copy_n(value.begin(), value.size(), &a[0]);
		value_ = std::move(a);
	}

	Cell::Cell(Bytes&& value)
	{ 
		if (value.size() > MAX_STRING_DATA)
			throw MaxLengthExceededException();

		Bytes a{std::byte(0)}; // Initialize array with zeros
		std::copy_n(value.begin(), value.size(), &a[0]);
		value_ = std::move(a);
	}

	ColumnType Cell::get_type() const
	{
		if (is_int())
			return ColumnTypeInt32;
		if (is_bool())
			return ColumnTypeBool;
		if (is_string())
			return ColumnTypeString;
		return ColumnTypeBytes;
	}

	bool Cell::operator< (const Cell& other) const
	{
		if (get_type() != other.get_type())
			throw TypeException();

		return *this < other;

	}

	bool Cell::is_int() const 
	{
    	return std::holds_alternative<Int32>(value_);
	}

	bool Cell::is_bool() const 
	{
	    return std::holds_alternative<Bool>(value_);
	}

	bool Cell::is_string() const 
	{
	    return std::holds_alternative<String>(value_);
	}

	bool Cell::is_bytes() const 
	{
	    return std::holds_alternative<Bytes>(value_);
	}


	Int32& Cell::get_int()
	{
		if (!this->is_int())
			throw TypeException();
		return std::get<Int32>(value_);
	}


	Bool& Cell::get_bool()
	{
		if (!this->is_bool())
			throw TypeException();
		return std::get<Bool>(value_);
	}


	String& Cell::get_string()
	{
		if (!this->is_string())
			throw TypeException();
		return std::get<String>(value_);
	}


	Bytes& Cell::get_bytes()
	{
		if (!this->is_bytes())
			throw TypeException();
		return std::get<Bytes>(value_);
	}
} //namespace memdb
