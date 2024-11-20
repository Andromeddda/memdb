#include "table.h"
#include <iomanip>

namespace memdb 
{
    //
    // Column description
    //
    ColumnDescription::ColumnDescription()
    : type_(ColumnTypeInt32), name_(), attributes_(0)
    {}

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

    bool ColumnDescription::operator== (const ColumnDescription& other) const {
        return name_ == other.name_;
    }


    //
    // Table
    //

    Table::Table(
        std::string&& name,
        columns_t&&  columns) :
    name_(name), columns_(columns), rows_(), index_()  {}

    Table::Table(
        const char* name,
        columns_t&&  columns) :
    name_(name), columns_(columns), rows_(), index_() {}

    std::string Table::name() { return name_; }


    // Print the table to the output stream
    void Table::display(std::ostream& os) {
        os << "TODO: display table " << std::quoted(name_) << '\n';
    }

    //
    // Modification methods
    //

    void Table::insert_row(row_t&& row) 
    {
        (void)row;
        // TODO
    }

    row_t Table::get_row(const std::string& index_column,
        const cell_t& index) 
    {
        (void)index;
        (void)index_column;
        // TODO
        return row_t();
    }


    void Table::delete_row(const std::string& index_column,
        const cell_t& index) 
    {
        (void)index;
        (void)index_column;
        // TODO
    }
} // memdb