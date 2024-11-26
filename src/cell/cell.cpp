#include "cell/cell.hpp"
#include <algorithm>

namespace memdb {
    #define UCHAR_TO_HEX(c) (((c) < (unsigned char)0xA) ? (char)((char)'0' + (char)(c)) : (char)((char)'A' + (char)(c) - (char)10))

    std::string byte_to_str(const std::byte& b)
    {
        std::string res = "";
        unsigned char c = (unsigned char)b;

        res = UCHAR_TO_HEX(c & 0xF) + res;
        res = UCHAR_TO_HEX((c >> 4) & 0xF) + res;

        return res;
    }

    static const std::unordered_map<CellType, std::string>
        type_to_str {
            {CellType::INT32, "Int32"},
            {CellType::BOOL, "Bool"},
            {CellType::STRING, "String"},
            {CellType::BYTES, "Bytes"}
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

    CellType Cell::get_type() const
    {
        if (is_int())
            return CellType::INT32;
        if (is_bool())
            return CellType::BOOL;
        if (is_string())
            return CellType::STRING;
        return CellType::BYTES;
    }

    bool Cell::less(const Cell& other) const
    {
        if (get_type() != other.get_type())
            throw TypeException();

        return this->value_ < other.value_;

    }

    size_t Cell::hash() const
    {
        CellType type = get_type();

        switch (type)
        {
        case CellType::INT32: return std::hash<int>{}(get_int());
        case CellType::BOOL : return std::hash<bool>{}(get_bool());
        case CellType::STRING: return std::hash<std::string>{}(get_string());
        default:
            auto bytes = get_bytes();
            std::string str = "";
            for (std::byte b : bytes)
                str += (char)b;
            return std::hash<std::string>{}(str);
        }
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
        CellType type = get_type();
        if (type != CellType::INT32)
            throw IncompatibleTypeOperatorException("-", type_to_str.at(type));
        return Cell(-get_int());
    }

    Cell Cell::operator-(const Cell& other) const
    {
        if (get_type() != CellType::INT32)
            throw IncompatibleTypeOperatorException("-", type_to_str.at(get_type()));
        if (other.get_type() != CellType::INT32)
            throw IncompatibleTypeOperatorException("-", type_to_str.at(other.get_type()));

        return Cell(get_int() - other.get_int());
    }


    Cell Cell::operator/ (const Cell& other) const
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 != CellType::INT32)
            throw IncompatibleTypeOperatorException("/", type_to_str.at(type1));
        if (type2 != CellType::INT32)
            throw IncompatibleTypeOperatorException("/", type_to_str.at(type2));

        if (!other.get_int())
            throw DivisionByZeroException();

        return Cell(get_int() / other.get_int());
    }

    Cell Cell::operator% (const Cell& other) const
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 != CellType::INT32)
            throw IncompatibleTypeOperatorException("%", type_to_str.at(type1));
        if (type2 != CellType::INT32)
            throw IncompatibleTypeOperatorException("%", type_to_str.at(type2));

        if (!other.get_int())
            throw DivisionByZeroException();

        return Cell(get_int() % other.get_int());
    }


    Cell Cell::operator* (const Cell& other) const
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 != CellType::INT32)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type1));
        if (type2 != CellType::INT32)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type2));

        if (!other.get_int())
            throw DivisionByZeroException();

        return Cell(get_int() * other.get_int());
    }

    Cell Cell::operator+ (const Cell& other) const // Int32 and String
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 != CellType::INT32 || type1 != CellType::STRING)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type1));
        if (type2 != CellType::INT32 || type2 != CellType::STRING)
            throw IncompatibleTypeOperatorException("*", type_to_str.at(type2));
        if (type1 != type2)
            throw DifferentTypesException("+");

        if (type1 == CellType::INT32)
            return Cell(get_int() * other.get_int());

        return Cell(get_string() + other.get_string());
    }

    // Logical operators (For Bool)
    Cell Cell::operator! () const
    {
        CellType type = get_type();
        if (type != CellType::BOOL)
            throw IncompatibleTypeOperatorException("!", type_to_str.at(type));
        return Cell(!get_bool());
    }

    Cell Cell::operator&& (const Cell& other) const
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 != CellType::BOOL)
            throw IncompatibleTypeOperatorException("&&", type_to_str.at(type1));
        if (type2 != CellType::BOOL)
            throw IncompatibleTypeOperatorException("&&", type_to_str.at(type2));

        return Cell(get_bool() && other.get_bool());
    }

    Cell Cell::operator|| (const Cell& other) const 
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 != CellType::BOOL)
            throw IncompatibleTypeOperatorException("||", type_to_str.at(type1));
        if (type2 != CellType::BOOL)
            throw IncompatibleTypeOperatorException("||", type_to_str.at(type2));

        return Cell(get_bool() || other.get_bool());
    }

    // Bitwise operators (For Int32, Bool and Bytes)
    Cell Cell::operator~ ()
    {   
        CellType type = get_type();
        if (type == CellType::STRING)
            throw IncompatibleTypeOperatorException("~", "String");

        if (type == CellType::INT32)
            return  Cell(~get_int());

        if (type == CellType::BOOL)
            return Cell(!get_bool());

        std::vector<std::byte> bt = get_bytes();

        for (auto &b : bt)
            b = ~b;
        return Cell(bt);
    }

    Cell Cell::operator| (const Cell& other) const
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 == CellType::STRING || type2 == CellType::STRING)
            throw IncompatibleTypeOperatorException("|", "String");

        if (type1 != type2)
            throw DifferentTypesException("|");
        if (size_ != other.size_)
            throw DifferentSizeException("|");

        if (type1 == CellType::INT32)
            return  Cell(get_int() | other.get_int());

        if (type1 == CellType::BOOL)
            return Cell(get_bool() | other.get_bool());

        std::vector<std::byte> bt1 = get_bytes();
        std::vector<std::byte> bt2 = get_bytes();

        for (auto i = 0LU; i < bt1.size(); ++i)
            bt1[i] |= bt2[i];
        return Cell(bt1);
    }

    Cell Cell::operator& (const Cell& other) const
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 == CellType::STRING || type2 == CellType::STRING)
            throw IncompatibleTypeOperatorException("&", "String");

        if (type1 != type2)
            throw DifferentTypesException("&");
        if (size_ != other.size_)
            throw DifferentSizeException("&");

        if (type1 == CellType::INT32)
            return  Cell(get_int() & other.get_int());

        if (type1 == CellType::BOOL)
            return Cell(get_bool() & other.get_bool());

        std::vector<std::byte> bt1 = get_bytes();
        std::vector<std::byte> bt2 = get_bytes();

        for (auto i = 0LU; i < bt1.size(); ++i)
            bt1[i] &= bt2[i];
        return Cell(bt1);
    }

    Cell Cell::operator^ (const Cell& other) const
    {
        CellType type1 = get_type();
        CellType type2 = other.get_type();

        if (type1 == CellType::STRING || type2 == CellType::STRING)
            throw IncompatibleTypeOperatorException("^", "String");

        if (type1 != type2)
            throw DifferentTypesException("^");
        if (size_ != other.size_)
            throw DifferentSizeException("^");

        if (type1 == CellType::INT32)
            return  Cell(get_int() ^ other.get_int());

        if (type1 == CellType::BOOL)
            return Cell(get_bool() ^ other.get_bool());

        std::vector<std::byte> bt1 = get_bytes();
        std::vector<std::byte> bt2 = get_bytes();

        for (auto i = 0LU; i < bt1.size(); ++i)
            bt1[i] ^= bt2[i];
        return Cell(bt1);
    }

    std::string Cell::ToString() const
    {
        CellType type = get_type();

        switch (type)
        {
        case CellType::BOOL: return get_bool() ? "true" : "false";
        case CellType::INT32: return std::to_string(get_int());
        case CellType::STRING: return get_string();
        default:
            std::string res = "0x";
            std::vector<std::byte> bts = get_bytes();
            for (std::byte &b : bts)
                res += byte_to_str(b);
            return res;
        }
    }
} //namespace memdb

