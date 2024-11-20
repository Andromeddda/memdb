#include "cell.h"

#include <algorithm>

namespace memdb {

    static const std::unordered_map<ColumnType, std::string>
        type_to_str {
            {ColumnTypeInt32, "Int32"},
            {ColumnTypeBool, "Bool"},
            {ColumnTypeString, "String"},
            {ColumnTypeBytes, "Bytes"}
        };

    Cell::Cell(Int32 value) : 
        value_(value), size_(4)
    { }

    Cell::Cell(Bool value) : 
        value_(value), size_(1)
    { }

    Cell::Cell(const std::string& value)
    {
        size_ = value.size();

        if (size_ > MAX_STRING_DATA)
            throw MaxLengthExceededException();

        String a{0}; // initialize with zeros
        std::copy_n(value.begin(), size_, &a[0]);
        value_ = std::move(a);

    }

    Cell::Cell(const std::vector<std::byte>& value)
    {
        size_ = value.size();
        if (size_ > MAX_STRING_DATA)
            throw MaxLengthExceededException();

        Bytes a{std::byte(0)}; // Initialize with zeros
        std::copy_n(value.begin(), size_, &a[0]);
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

    bool Cell::less(const Cell& other) const
    {
        if (get_type() != other.get_type())
            throw TypeException();

        return this->value_ < other.value_;

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


    Int32 Cell::get_int() const
    {
        if (!this->is_int())
            throw TypeException();
        return std::get<Int32>(value_);
    }


    Bool Cell::get_bool() const
    {
        if (!this->is_bool())
            throw TypeException();
        return std::get<Bool>(value_);
    }


    std::string Cell::get_string() const
    {
        if (!this->is_string())
            throw TypeException();
        String ret = std::get<String>(value_);
        return std::string(ret.begin(), ret.begin() + size_);
    }


    std::vector<std::byte> Cell::get_bytes() const
    {
        if (!this->is_bytes())
            throw TypeException();
        Bytes ret = std::get<Bytes>(value_);
        return std::vector<std::byte>(ret.begin(), ret.begin() + size_);
    }


    // Comparison operators
    Cell Cell::operator== (const Cell& other) const
    {
        return Cell(this->value_ == other.value_);
    }

    Cell Cell::operator!= (const Cell& other) const
    {
        return Cell(this->value_ != other.value_);
    }

    Cell Cell::operator>= (const Cell& other) const
    {
        return Cell(this->value_ >= other.value_);
    }

    Cell Cell::operator<= (const Cell& other) const
    {
        return Cell(this->value_ <= other.value_);
    }

    Cell Cell::operator< (const Cell& other) const
    {
        return Cell(this->value_ < other.value_);
    }

    Cell Cell::operator> (const Cell& other) const
    {
        return Cell(this->value_ > other.value_);
    }

    // Arithmetical operators (For Int32)
    Cell Cell::operator- () const
    {    
        ColumnType type = get_type();
        if (type != ColumnTypeInt32)
            throw IncompatibleTypeOperatorException("-", type_to_str.at(type));
        return Cell(-get_int());
    }

    Cell Cell::operator-(const Cell& other) const
    {
        if (get_type() != ColumnTypeInt32)
            throw IncompatibleTypeOperatorException("-", type_to_str.at(get_type()));
        if (other.get_type() != ColumnTypeInt32)
            throw IncompatibleTypeOperatorException("-", type_to_str.at(other.get_type()));

        return Cell(get_int() - other.get_int());
    }


    Cell Cell::operator/ (const Cell& other) const
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 != ColumnTypeInt32)
            throw IncompatibleTypeOperatorException("/", type_to_str.at(type1));
        if (type2 != ColumnTypeInt32)
            throw IncompatibleTypeOperatorException("/", type_to_str.at(type2));

        if (!other.get_int())
            throw DivisionByZeroException();

        return Cell(get_int() / other.get_int());
    }


    Cell Cell::operator* (const Cell& other) const
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 != ColumnTypeInt32)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type1));
        if (type2 != ColumnTypeInt32)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type2));

        if (!other.get_int())
            throw DivisionByZeroException();

        return Cell(get_int() * other.get_int());
    }

    Cell Cell::operator+ (const Cell& other) const // Int32 and String
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 != ColumnTypeInt32 || type1 != ColumnTypeString)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type1));
        if (type2 != ColumnTypeInt32 || type2 != ColumnTypeString)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type2));
        if (type1 != type2)
            throw DifferentTypesException("+");

        if (type1 == ColumnTypeInt32)
            return Cell(get_int() * other.get_int());

        return Cell(get_string() + other.get_string());
    }

    // Logical operators (For Bool)
    Cell Cell::operator! () const
    {
        ColumnType type = get_type();
        if (type != ColumnTypeBool)
            throw IncompatibleTypeOperatorException("!", type_to_str.at(type));
        return Cell(!get_bool());
    }

    Cell Cell::operator&& (const Cell& other) const
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 != ColumnTypeBool)
            throw IncompatibleTypeOperatorException("&&", type_to_str.at(type1));
        if (type2 != ColumnTypeBool)
            throw IncompatibleTypeOperatorException("&&", type_to_str.at(type2));

        return Cell(get_bool() && other.get_bool());
    }

    Cell Cell::operator|| (const Cell& other) const 
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 != ColumnTypeBool)
            throw IncompatibleTypeOperatorException("||", type_to_str.at(type1));
        if (type2 != ColumnTypeBool)
            throw IncompatibleTypeOperatorException("||", type_to_str.at(type2));

        return Cell(get_bool() || other.get_bool());
    }

    // Bitwise operators (For Int32, Bool and Bytes)
    Cell Cell::operator~ ()
    {   
        ColumnType type = get_type();
        if (type == ColumnTypeString)
            throw IncompatibleTypeOperatorException("~", "String");

        if (type == ColumnTypeInt32)
            return  Cell(~get_int());

        if (type == ColumnTypeBool)
            return Cell(!get_bool());

        std::vector<std::byte> bt = get_bytes();

        for (auto &b : bt)
            b = ~b;
        return Cell(bt);
    }

    Cell Cell::operator| (const Cell& other) const
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 == ColumnTypeString || type2 == ColumnTypeString)
            throw IncompatibleTypeOperatorException("|", "String");

        if (type1 != type2)
            throw DifferentTypesException("|");
        if (size_ != other.size_)
            throw DifferentSizeException("|");

        if (type1 == ColumnTypeInt32)
            return  Cell(get_int() | other.get_int());

        if (type1 == ColumnTypeBool)
            return Cell(get_bool() | other.get_bool());

        std::vector<std::byte> bt1 = get_bytes();
        std::vector<std::byte> bt2 = get_bytes();

        for (auto i = 0LU; i < bt1.size(); ++i)
            bt1[i] |= bt2[i];
        return Cell(bt1);
    }

    Cell Cell::operator& (const Cell& other) const
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 == ColumnTypeString || type2 == ColumnTypeString)
            throw IncompatibleTypeOperatorException("&", "String");

        if (type1 != type2)
            throw DifferentTypesException("&");
        if (size_ != other.size_)
            throw DifferentSizeException("&");

        if (type1 == ColumnTypeInt32)
            return  Cell(get_int() & other.get_int());

        if (type1 == ColumnTypeBool)
            return Cell(get_bool() & other.get_bool());

        std::vector<std::byte> bt1 = get_bytes();
        std::vector<std::byte> bt2 = get_bytes();

        for (auto i = 0LU; i < bt1.size(); ++i)
            bt1[i] &= bt2[i];
        return Cell(bt1);
    }

    Cell Cell::operator^ (const Cell& other) const
    {
        ColumnType type1 = get_type();
        ColumnType type2 = other.get_type();

        if (type1 == ColumnTypeString || type2 == ColumnTypeString)
            throw IncompatibleTypeOperatorException("^", "String");

        if (type1 != type2)
            throw DifferentTypesException("^");
        if (size_ != other.size_)
            throw DifferentSizeException("^");

        if (type1 == ColumnTypeInt32)
            return  Cell(get_int() ^ other.get_int());

        if (type1 == ColumnTypeBool)
            return Cell(get_bool() ^ other.get_bool());

        std::vector<std::byte> bt1 = get_bytes();
        std::vector<std::byte> bt2 = get_bytes();

        for (auto i = 0LU; i < bt1.size(); ++i)
            bt1[i] ^= bt2[i];
        return Cell(bt1);
    }
} //namespace memdb
