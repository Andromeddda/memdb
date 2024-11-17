#include "table.h"

namespace memdb 
{
    ColumnDescription::ColumnDescription(
        ColumnType            type, 
        const std::string&    name, 
        unsigned char         attributes) 
    : type_(type), name_(name), attributes_(attributes) {}


    ColumnDescription::ColumnDescription(
        ColumnType          type, 
        const char*         name, 
        unsigned char       attributes) 
    : type_(type), name_(name), attributes_(attributes) {}


    Table::Table(
        std::string&& name,
        std::vector<ColumnDescription>&& columns) :
    name_(name), columns_(columns), rows_(), index_()  {}

    Table::Table(
        const char* name,
        std::vector<ColumnDescription>&& columns) :
    name_(name), columns_(columns), rows_(), index_() {}

    size_t Hash::operator() (const cell_t& cell) const {
        String arr = std::get<String>(cell); 
        std::string str = std::string(arr.begin(), arr.end());
        return std::hash<std::string>{}(str);
    }


    std::string Table::name() 
    {
        return name_;
    }

    
} // memdb