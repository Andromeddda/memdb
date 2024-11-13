#include "cell.h"
#include <iostream>

namespace memdb {

    // Macro for building classes derived from Cell
    // The macro includes:
    //      The definition of derived class, for example class Int32Cell
    //      The definition of it's nested class CellData, derived from Cell::CellData
    //      The definition of Cell constructor using given type of data

    #define CELL_DERIVED(name, type, to_string_body)        \
    class name##Cell : public Cell {                        \
    protected:                                              \
        class CellData;                                     \
        friend class Cell;                                  \
    };                                                      \
                                                            \
    class name##Cell::CellData : public Cell::CellData {    \
    public:                                                 \
        CellData() : value_(type()) {};                     \
        CellData(type value) : value_(value) {};            \
        std::string ToString() override to_string_body      \
        Cell::CellData* Copy() {                            \
            return new CellData(value_);                    \
        }                                                   \
    private:                                                \
        type value_;                                        \
    };                                                      \
    Cell::Cell(type value) :                                \
        data_(new name##Cell::CellData(value)),             \
        type_(name)                                         \
    { }

    //
    // Declaration of derived classes
    // 

    // Bool
    CELL_DERIVED(Bool, bool, { return value_ ? "true" : "false"; })

    // Int32
    CELL_DERIVED(Int32, int, { return std::to_string(value_); })

    // String
    CELL_DERIVED(String, std::string, { return value_; })

    Cell::CellData::CellData() { }
    Cell::CellData::~CellData() { }

    std::string Cell::CellData::ToString() { return std::string(); }

    // Default constructor
    Cell::Cell() :
        data_(nullptr), 
        type_(Null) 
    { }

    // Move costructor
    Cell::Cell(Cell&& other) 
    {
        data_ = other.data_;
        type_ = other.type_;
        other.data_ = nullptr;
        other.type_ = Null;
    }

    // Copy constructor
    Cell::Cell(const Cell& other) :
        data_(other.data_ ? other.data_->Copy() : nullptr),
        type_(other.type_)
    { }

    // Delegate char* to string so that string literal won't be misinterpret as bool
    Cell::Cell(const char *data) : 
        Cell(std::string(data)) 
    { }

    // Destructor
    Cell::~Cell()
    {
        if (data_ != nullptr)
            delete data_;
        data_ = nullptr;
    }

    // String representation
    std::string Cell::DataToString() {
        if (!data_) return "";
        return data_->ToString();
    }

} // namespace memdb
