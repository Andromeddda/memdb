#include "database/table.hpp"

namespace memdb
{
    Table::Table(const std::string& table_name, 
            const ColumnMap& columns) :
    name_(table_name), columns_(columns), rows_()  { }

    Table::Table(const char* table_name, 
            const ColumnMap& columns) :
    name_(table_name), columns_(columns), rows_()  { }

    std::string Table::name() 
    {
        return name_; 
    }

    // Print the table to the output stream
    void Table::display(std::ostream& os) {
        (void)os;
    }

    size_t Table::width() const 
    {
        return columns_.size();
    }

    size_t Table::column_index(const std::string& column_name)
    {
        return columns_.at(column_name);
    }


} // namespace memdb