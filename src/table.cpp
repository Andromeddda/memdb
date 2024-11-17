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

    
} // memdb