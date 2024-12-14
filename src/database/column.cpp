#include "database/column.hpp"

namespace memdb 
{
    Column::Column()
    : type_(CellType::INT32), name_(), attributes_(0) //, index_()
    { }

    Column::Column(const char *name)
    : type_(CellType::INT32), name_(name), attributes_(0) //, index_()
    { }

    Column::Column(const std::string& name)
    : type_(CellType::INT32), name_(name), attributes_(0) //, index_()
    { }

    Column::Column(
        CellType              type, 
        const std::string&    name, 
        unsigned char         attributes) 
    : type_(type), name_(name), attributes_(attributes) //, index_() 
    { }

    Column::Column(
        CellType            type, 
        const char*         name, 
        unsigned char       attributes) 
    : type_(type), name_(name), attributes_(attributes) //, index_() 
    {}

    bool Column::operator== (const Column& other) const {
        return name_ == other.name_;
    }

} // namespace memdb